#ifndef __PAS_RUNNER_HPP__
#define __PAS_RUNNER_HPP__

#include "runner_base.hpp"

class pas_runner: public runner_base
{
public:
    
    pas_runner();

private:

    void _execTarget();
    bool _checkFilePrivilege(const char*);

};

#endif
