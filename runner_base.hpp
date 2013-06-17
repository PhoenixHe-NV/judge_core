#ifndef __RUNER_BASE_HPP__
#define __RUNER_BASE_HPP__

#include <sys/resource.h>
#include <unistd.h>
#include <stdint.h>
#include <condition_variable>
#include <thread>
#include "common.hpp"

class runner_base
{
public:

    struct result_t{
        int32_t timeCost, memoryCost, result, returnCode;
    };

    runner_base();
    runner_base(const runner_base&) = delete;
    runner_base& operator=(const runner_base&) = delete;

    result_t run();

private:

    void _forkChild();
    inline void _continueLoop();
    inline void _stopLoop();
    inline void _peekReg(struct user_regs_struct*);
    bool _checkExit(int);
    void _getStringArg(long, long*);
    bool _checkSyscall(struct user_regs_struct*);
    void _work();
    void _summarize();
    void _alarmTimer();
    void _updateTimeUsage();
    inline bool _updateMemUsage();

    bool _intoCall = 1;
    bool _checkMem = 0;
    pid_t _childpid = 0;
    struct rusage _ur;
    condition_variable_any _cv;
    mutex _cv_m;
    bool _running = 0;

protected:

    virtual void _execTarget() = 0;
    virtual bool _checkFilePrivilege(const char*) = 0;

	int32_t _syscallQuota[syscallMaxNum];
    result_t _res;

};

#endif
