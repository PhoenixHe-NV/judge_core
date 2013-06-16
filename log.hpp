#ifndef __LOG_HPP__
#define __LOG_HPP__

#include <iostream>

#ifdef DISABLE_LOG
#define LOG(x)
#else
#define LOG(x) std::cerr<<x<<std::endl
#endif

#endif
