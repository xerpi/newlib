/*

Copyright (C) 2019, Martin Larralde

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

*/

#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>

#include <psp2/rtc.h>
#include <psp2/kernel/processmgr.h>

#include "vitaerror.h"

int clock_gettime(clockid_t clk_id, struct timespec *tp)
{
    time_t seconds;
    SceDateTime time;
    uint64_t t;

    if (!tp) {
        errno = EFAULT;
        return -1;
    }
    switch(clk_id) {
        case CLOCK_REALTIME:
            sceRtcGetCurrentClockLocalTime(&time);

            sceRtcGetTime_t(&time, &seconds);

            tp->tv_sec = seconds;
            tp->tv_nsec = time.microsecond * 1000;
            return 0;
            break;
        case CLOCK_MONOTONIC:
            t = sceKernelGetProcessTimeWide();
            tp->tv_sec = t / 1000000;
            tp->tv_nsec = (t % 1000000) * 1000;
            return 0;
            break;
        default:
            break;
    }
    errno = EINVAL;
    return -1;
}

int clock_settime(clockid_t clk_id, const struct timespec *tp)
{
    if (!tp) {
        errno = EFAULT;
        return -1;
    }
    errno = EPERM;
    return -1;
}

int clock_getres(clockid_t clk_id, struct timespec *res)
{
    if (!res) {
        errno = EFAULT;
        return -1;
    }

    switch(clk_id) {
        case CLOCK_REALTIME:
            res->tv_sec = 0;
            res->tv_nsec = 1000; // 1 microsecond
            return 0;
            break;
        case CLOCK_MONOTONIC:
            res->tv_sec = 0;
            res->tv_nsec = 1000; // 1 microsecond
            return 0;
            break;
        default:
            break;
    }

    errno = EINVAL;
    return -1;
}
