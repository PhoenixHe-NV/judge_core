#ifndef __C_RUNNER_HPP__
#define __C_RUNNER_HPP__

#include "runner_base.hpp"

class c_runner: public runner_base
{
public:
    
    c_runner();

private:

    void _execTarget();
    bool _checkFilePrivilege(const char*);

};

#endif
