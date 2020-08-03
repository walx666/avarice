/*
 *	avarice - The "avarice" program.
 *  Copyright (C) 2019 Marius Greuel
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License Version 2
 *	as published by the Free Software Foundation.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
 *
 * $Id$
 */

#include "unistd.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

class MicroSleep
{
public:
    MicroSleep()
    {
        if (::timeBeginPeriod(timerPeriod) == TIMERR_NOERROR)
        {
            m_resetTimerPeriod = true;
        }
    }

    ~MicroSleep()
    {
        if (m_resetTimerPeriod)
        {
            ::timeEndPeriod(timerPeriod);
        }
    }

    int Sleep(DWORD us)
    {
        if (us == 0)
        {
            return 0;
        }

        LARGE_INTEGER frequency;
        if (QueryPerformanceFrequency(&frequency))
        {
            LARGE_INTEGER start;
            QueryPerformanceCounter(&start);

            if (us > 10000)
            {
                ::Sleep((us - 5000) / 1000);
            }

            LARGE_INTEGER end;
            end.QuadPart = start.QuadPart + (frequency.QuadPart * us / 1000000);

            while (true)
            {
                LARGE_INTEGER current;
                QueryPerformanceCounter(&current);
                if (current.QuadPart >= end.QuadPart)
                {
                    break;
                }
            }
        }
        else
        {
            ::Sleep((us / 1000) + 1);
        }

        return 0;
    }

private:
    static const UINT timerPeriod = 1; // 1ms
    bool m_resetTimerPeriod = false;
};

int usleep(unsigned int us)
{
    static MicroSleep microSleep;
    return microSleep.Sleep(us);
}

pid_t fork(void)
{
    return -1;
}

pid_t setsid(void)
{
    return -1;
}

int fcntl(int fd, int cmd, ... /* arg */)
{
    return -1;
}
