#include "native_runner.hpp"
#include "common.hpp"

#include <unistd.h>

DECLARE_ARG(string, cmd);

native_runner::native_runner():
    runner_base()
{
	_syscallQuota[__NR_access]		    =   -1;
	_syscallQuota[__NR_brk]		    	=   -1;
	_syscallQuota[__NR_close]    		=   -1;
	_syscallQuota[__NR_fstat]		    =   -1;
	_syscallQuota[__NR_mmap]	    	=   -1;
	_syscallQuota[__NR_mprotect]    	=   -1;
	_syscallQuota[__NR_munmap]		    =   -1;
	_syscallQuota[__NR_open]		    =   -1;
	_syscallQuota[__NR_read]		    =   -1;
	_syscallQuota[__NR_write]		    =   -1;
	_syscallQuota[__NR_uname]		    =   -1;
	_syscallQuota[__NR_rt_sigprocmask]	=   -1;
	_syscallQuota[__NR_tgkill]		    =   -1;
	_syscallQuota[__NR_gettid]		    =   -1;
	_syscallQuota[__NR_time]		    =   -1;
	_syscallQuota[__NR_arch_prctl]	    =   2;
	_syscallQuota[__NR_execve]		    =   1;
	_syscallQuota[__NR_exit]		    =   1;
	_syscallQuota[__NR_exit_group]	    =   1;
}

void native_runner::_execTarget()
{ execl(ARG_cmd.c_str(), ARG_cmd.c_str(), NULL); }

bool native_runner::_checkFilePrivilege(const char*)
{ return 0; }
