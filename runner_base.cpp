#include <signal.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <chrono>
#include "runner_base.hpp"
#include "common.hpp"
#include "log.hpp"

DEFINE_ARG(string, cmd, "The command you want to run");
DEFINE_OPTIONAL_ARG(string, type, "native", "native/python, syscall limit type");
DEFINE_OPTIONAL_ARG(string, input, "/dev/stdin", "Input File");
DEFINE_OPTIONAL_ARG(string, output, "/dev/stdout", "Output File");
DEFINE_OPTIONAL_ARG(int, tl, 1000, "Time limit(ms)");
DEFINE_OPTIONAL_ARG(int, ml, 64*1024, "Memory limit(kB)");
DEFINE_OPTIONAL_ARG(int, sl, 16*1024, "Stack limit(kB)");
DEFINE_OPTIONAL_ARG(int, ol, 1024, "Output limit(Byte)");
DEFINE_OPTIONAL_ARG(int, uid, 1000, "The uid for executing the program to be judged");
DEFINE_OPTIONAL_ARG(int, gid, 1000, "The gid for executing the program to be judged");
DEFINE_ARG(string, root, "The root directory of the judger");

class ForgetExecException { };

runner_base::runner_base()
{
    memset(_syscallQuota, 0, sizeof(_syscallQuota));
    memset(&_res, 0, sizeof(_res));
    _res.result = -1;
}

runner_base::result_t runner_base::run()
{
    _work();
    _updateResUsage();
    _summarize();
    return _res;
}

void runner_base::_work()
{
    _forkChild();
    int sta;
    struct user_regs_struct regs;
    wait4(_childpid, &sta, 0, &_ur);     //skip exec
    _running = 1;
    thread alarmTimer(&runner_base::_alarmTimer, this);
    _updateResUsage();
    while (1)
    {
        _continueLoop();
        wait4(_childpid, &sta, 0, &_ur);
        if (_checkExit(sta))
            break;
        if (_intoCall^=1)
            continue;
        _peekReg(&regs);
        if (_checkSyscall(&regs))
        {
            _res.result = RES_RE;
            break;
        }
    }
    _running = 0;
    _cv.notify_all();
    alarmTimer.join();
    _stopLoop();
    _res.returnCode = WEXITSTATUS(sta);
}

void runner_base::_alarmTimer()
{
    unique_lock<std::mutex> lk(_cv_m);
    if (!_cv.wait_for(lk, chrono::milliseconds(ARG_tl + 10),
                [&](){ return _running == 0; }))
        kill(_childpid, SIGXCPU);
}

void runner_base::_summarize()
{
    minimize(_res.timeCost, ARG_tl);
    switch (_res.result)
    {
        case RES_RE:
            break;
        case RES_TLE:
            _res.timeCost = ARG_tl;
            break;
        case RES_OK:
        case RES_OLE:
            break;
        default:
            _res.result = RES_RE;
    }
		
	for (int i=0;i<syscallMaxNum;i++){
		if (_syscallQuota[i]!=0){
			LOG("quota left "<< syscallNames[i]<< ": "<< _syscallQuota[i]);
		}
	}
}

void runner_base::_forkChild()
{
    pid_t pid=fork();
    if (pid == 0)
    {
        chdir(ARG_root.c_str());

        /*
        close(0);
        auto fd_input = open(ARG_input.c_str(), O_RDONLY, 0777);
        dup2(fd_input, 0);
        close(fd_input);
        close(1);
        auto fd_output = open(ARG_output.c_str(), O_RDWR|O_CREAT|O_TRUNC, 0777);
        dup2(fd_output, 1);
        close(fd_output);
        */

        freopen(ARG_input.c_str(), "r", stdin);
        freopen(ARG_output.c_str(), "w", stdout);

        setgid(ARG_gid);
        setuid(ARG_uid);

        rlimit limit;
        getrlimit(RLIMIT_AS, &limit);
        limit.rlim_cur = limit.rlim_max = ARG_ml*1024;
        setrlimit(RLIMIT_AS, &limit);

        getrlimit(RLIMIT_STACK, &limit);
        limit.rlim_cur = limit.rlim_max = ARG_sl*1024;
        setrlimit(RLIMIT_STACK,&limit);

        getrlimit(RLIMIT_FSIZE, &limit);
        limit.rlim_cur = limit.rlim_max = ARG_ol;
        setrlimit(RLIMIT_FSIZE, &limit);

        getrlimit(RLIMIT_NPROC, &limit);
        limit.rlim_cur = limit.rlim_max = 2;
        setrlimit(RLIMIT_NPROC, &limit);

        ptrace(PTRACE_TRACEME,0,NULL,NULL);
        LOG("Rlimit was set. starting target command.");
        _execTarget();
        throw ForgetExecException();
    }
    else
    {
        _childpid = pid;
    }
}

void runner_base::_continueLoop()
{ ptrace(PTRACE_SYSCALL, _childpid, NULL, NULL); }

void runner_base::_stopLoop()
{ kill(_childpid, SIGKILL); }

void runner_base::_peekReg(struct user_regs_struct* regs)
{ ptrace(PTRACE_GETREGS, _childpid, NULL, regs); }

void runner_base::_updateResUsage()
{
    maximize(_res.memoryCost, (int32_t)_ur.ru_maxrss);
    tms timesvalue;
    times(&timesvalue);
    clock_t childuser = timesvalue.tms_cutime;
    long clk_tck = sysconf(_SC_CLK_TCK);
    int32_t timeCost = int32_t(double(childuser)/clk_tck*1000);
    //int32_t timeCost = _ur.ru_utime.tv_sec*1000 + _ur.ru_utime.tv_usec/1000;
    maximize(_res.timeCost, timeCost);
    //LOG("update :"<< _ur.ru_maxrss<< " "<< timeCost);
}

bool runner_base::_checkExit(int sta)
{
    if (WIFSTOPPED(sta))
        switch (WSTOPSIG(sta))
        {
            case SIGTRAP: 
                return 0;
            case SIGXCPU:
                _res.result = RES_TLE;
                return 1;
            case SIGXFSZ:
                _res.result = RES_OLE;
                return 1;
            case SIGFPE:
            case SIGSEGV:
            default:
                _res.result = RES_RE;
                return 1;
    }

    if (WIFSIGNALED(sta))
    {
        LOG("oooops...");
        _res.result = RES_RE;
        return 1;
    }
    if (WIFEXITED(sta))
    {
        _res.result = RES_OK;
        LOG("exit normally");
        return 1;
    }
    return 0;
}

void runner_base::_getStringArg(long addr, long* buf)
{
    for (int i=0;i<10;i++)
        buf[i] = ptrace(PTRACE_PEEKDATA, _childpid, addr + i*sizeof(long), NULL);
    buf[10]=-1;buf[11]=0;
}

bool runner_base::_checkSyscall(struct user_regs_struct* regs)
{
    long callID = regs->orig_rax;
    if (callID == __NR_read || callID == __NR_write)
        return 0;
    _updateResUsage();
    if (callID>=syscallMaxNum || callID<0)
    {
        LOG("ILLEGAL syscall code:"<< callID);
        return 1;
    }
    //if (callID == __NR_open || callID == __NR_access || callID == __NR_readlink){
    if (callID == __NR_open || callID == __NR_access){
        long buf[12];
        _getStringArg(regs->rdi, buf);
        if (_checkFilePrivilege((char*)buf))
        {
            LOG("legal "<< syscallNames[callID]<< ": "<< (char*)buf);
            return 0;
        }
        else
        {
            LOG("ILLEGAL "<< syscallNames[callID]<< ": "<< (char*)buf);
            return 1;
        }
    }
    if ((_syscallQuota[callID]--))
    {
        //if (syscallQuota[orig_rax]>=0)
        LOG("legal sys call "<< callID<< ": "<< syscallNames[callID]);
        return 0;
    }
    else 
    {
        LOG("ILLEGAL sys call "<< callID<< ": "<< syscallNames[callID]);
        return 1;
    }
    return 0;
}
