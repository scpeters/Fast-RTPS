#ifndef DBGTIMEUTIL_H
#define DBGTIMEUTIL_H

#include <fastdds/dds/log/DbgTime.hpp>

#ifdef _WIN32
__declspec(dllimport) DbgTime* __cdecl get_dbg_time();
#else
DbgTime* get_dbg_time();
#endif

#define dbg_time get_dbg_time

#endif