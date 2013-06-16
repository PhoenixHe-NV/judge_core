#ifndef __NATIVE_RUNNER_HPP__
#define __NATIVE_RUNNER_HPP__

#include "runner_base.hpp"

class native_runner: public runner_base
{
public:
    
    native_runner();

private:

    void _execTarget();
    bool _checkFilePrivilege(const char*);

};

#endif
