#include "pas_runner.hpp"
#include "common.hpp"
#include "log.hpp"

#include <unistd.h>
#include <string.h>

DECLARE_ARG(string, cmd);

pas_runner::pas_runner():
    runner_base()
{
	_syscallQuota[__NR_close]    		=   -1;
	_syscallQuota[__NR_time]		    =   -1;
    _syscallQuota[__NR_fstat]           =   -1;
    _syscallQuota[__NR_poll]            =   -1;
    _syscallQuota[__NR_lseek]           =   -1;
	_syscallQuota[__NR_mmap]	    	=   -1;
	_syscallQuota[__NR_mprotect]    	=   -1;
	_syscallQuota[__NR_munmap]		    =   -1;
	_syscallQuota[__NR_brk]		    	=   -1;
	_syscallQuota[__NR_pread64]	    	=   -1;
	_syscallQuota[__NR_pwrite64]    	=   -1;
	_syscallQuota[__NR_mremap]       	=   -1;
	_syscallQuota[__NR_mincore]       	=   -1;
	_syscallQuota[__NR_madvise]       	=   -1;
	_syscallQuota[__NR_sendfile]       	=   -1;
    _syscallQuota[__NR_gettimeofday]    =   -1;
    _syscallQuota[__NR_getrlimit]       =   -1;
    _syscallQuota[__NR_getrusage]       =   -1;
    _syscallQuota[__NR_times]           =   -1;
	_syscallQuota[__NR_mlock]		    =   -1;
	_syscallQuota[__NR_munlock]		    =   -1;
	_syscallQuota[__NR_mlockall]	    =   -1;
	_syscallQuota[__NR_munlockall]	    =   -1;
    _syscallQuota[__NR_time]            =   -1;
    _syscallQuota[__NR_futex]           =   -1;
    _syscallQuota[__NR_remap_file_pages]=   -1;

	_syscallQuota[__NR_ioctl]	    	=   -1;

    _syscallQuota[__NR_rt_sigaction]    =   4;
	_syscallQuota[__NR_exit]		    =   1;
	_syscallQuota[__NR_exit_group]	    =   1;

    _syscallQuota[__NR_open]            =   FILE_CHECK_SYSCALL;
    _syscallQuota[__NR_stat]            =   FILE_CHECK_SYSCALL;
    _syscallQuota[__NR_lstat]           =   FILE_CHECK_SYSCALL;
    _syscallQuota[__NR_access]          =   FILE_CHECK_SYSCALL;
    _syscallQuota[__NR_readlink]        =   FILE_CHECK_SYSCALL;
}

void pas_runner::_execTarget()
{ execl(ARG_cmd.c_str(), ARG_cmd.c_str(), NULL); }

const char* _pas_whilelist[]=
{
    "/etc/timezone",
    "/usr/share/zoneinfo/Asia/Shanghai"
};

bool pas_runner::_checkFilePrivilege(const char* f)
{ 
    if (strncmp("/proc/", f, 6) == 0) return 1; 
    for (int i = 0;i < 2; ++i)
    {
        LOG(_pas_whilelist[i]);
        if (strcmp(f, _pas_whilelist[i]) == 0) return 1;
        LOG("NOT PASS");
    }
    return 0;
}
