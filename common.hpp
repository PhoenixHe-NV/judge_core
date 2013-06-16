#ifndef __COMMON_HPP__
#define __COMMON_HPP__

#include <stdio.h>
#include <sys/syscall.h>
#include "args.hpp"

DECLARE_ARG(string, cmd);
DECLARE_ARG(string, type);

template<typename T>
inline T maximum(const T a,const T b)
{ return a > b? a : b; }
template<typename T>
inline T minimum(const T a,const T b)
{ return a < b? a : b; }
template<typename T>
inline void maximize(T& a,const T b)
{ if (b > a) a = b; }
template<typename T>
inline void minimize(T& a,const T b)
{ if (b < a) a = b; }

const auto syscallMaxNum=314;

extern const char* const syscallNames[syscallMaxNum];

extern const char* signalNames[];

typedef enum 
{
    RES_OK      = 0,
    RES_AC      = 1,
    RES_WA      = 2,
    RES_OLE     = 3,
    RES_PE      = 4,
    RES_VE      = 5,
    RES_RE      = 6,
    RES_TLE     = 7,
    RES_MLE     = 8,
    RES_CE      = 9
}resultReference;

#endif
