#include "c_runner.hpp"
#include "common.hpp"

#include <unistd.h>

DECLARE_ARG(string, cmd);

c_runner::c_runner():
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

	_syscallQuota[__NR_uname]		    =   1;
	_syscallQuota[__NR_arch_prctl]	    =   1;
	_syscallQuota[__NR_exit]		    =   1;
	_syscallQuota[__NR_exit_group]	    =   1;
}

void c_runner::_execTarget()
{ execl(ARG_cmd.c_str(), ARG_cmd.c_str(), NULL); }

bool c_runner::_checkFilePrivilege(const char*)
{ return 0; }
