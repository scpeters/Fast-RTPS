#ifndef DBG_TIME_HPP
#define DBG_TIME_HPP

#include <stdint.h>
#include <vector>
#include <string>
#include <memory>
#include <cstdio>
#include <chrono>
#include <thread>

class DbgTime
{
public:

    bool thread_id_filter_;
    std::thread::id thread_id_;

    void set_thread_filter_to_this()
    {
        thread_id_filter_ = true;
        thread_id_ = std::this_thread::get_id();
    }

    void set_thread_filter_to_void()
    {
        thread_id_filter_ = true;
        thread_id_ = std::thread::id();
    }

    void set_thread_filter_to_off()
    {
        thread_id_filter_ = false;
    }

    DbgTime()
    {
        memset(time, 0, sizeof(time));
        thread_id_filter_ = true;
    }

    ~DbgTime()
    {
        print_meassures();
    }

    std::chrono::high_resolution_clock::time_point create_new_change0;
    std::chrono::high_resolution_clock::time_point intraprocess_delivery0;
    std::chrono::high_resolution_clock::time_point notify_wait_for_unread_cache_;

    enum timeblocks
    {
        create_new_change,
        serialize,
        intraprocess_delivery,
        notify_wait_for_unread_cache,
        take_next_data,
        take_next_data_deserialize,
        take_next_data_fails,
        TB_UPPERBOUND
    };

    struct Meassure
    {
        uint32_t calls;
        uint64_t time;
    };

    Meassure time[TB_UPPERBOUND];

    bool count_allowed()
    {
        return !thread_id_filter_ || (thread_id_filter_ && thread_id_ == std::this_thread::get_id());
    }

    void print_meassures()
    {
        static const char* names[] = {
            "create_new_change", 
            "->serialize",
            "->intraprocess_delivery",
            "notify_wait_for_unread_cache",
            "take_next_data",
            "->take_next_data_deserialize",
            "take_next_data_fails"
        };

        for(int i = 0; i < TB_UPPERBOUND; i++)
        {
            if (time[i].calls > 0)
            {
                printf("%s called %u total %.3f(us) avg_call_time %.3f(us)\n", names[i]
                    , time[i].calls, time[i].time / 1000.0, (time[i].time / time[i].calls) / 1000.0);
            }
        }
    }
};

#define DBGT dbg_time()
#define DBGT_ENABLE(PARAM_BOOL) dbg_time()->enabled_ = PARAM_BOOL
using DBGTCK = std::chrono::high_resolution_clock;

#define DBGT_SET(VAR, VAL) \
    if (dbg_time()->count_allowed()) \
    { \
        dbg_time()->VAR = VAL; \
    } \

#define DBGT_COUNT_DIFF(BLOCK, T0, T1) \
    if (dbg_time()->count_allowed()) \
    { \
        dbg_time()->time[DbgTime::BLOCK].calls++; \
        dbg_time()->time[DbgTime::BLOCK].time += (T1 - T0).count(); \
    } \

#define DBGT_COUNT_DIFF_FORCE(BLOCK, T0, T1) \
{ \
    dbg_time()->time[DbgTime::BLOCK].calls++; \
    dbg_time()->time[DbgTime::BLOCK].time += (T1 - T0).count(); \
} \


#endif