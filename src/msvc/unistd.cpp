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
#include <unordered_map>
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

class Handles
{
public:
    Handles()
    {
        InitializeSRWLock(&m_lock);
    }

    int Add(HANDLE handle)
    {
        int fd = GetNextHandle();

        AcquireSRWLockExclusive(&m_lock);
        m_map.emplace(fd, handle);
        ReleaseSRWLockExclusive(&m_lock);

        return fd;
    }

    void Remove(int fd)
    {
        AcquireSRWLockExclusive(&m_lock);
        m_map.erase(fd);
        ReleaseSRWLockExclusive(&m_lock);
    }

    HANDLE Lookup(int fd)
    {
        HANDLE handle = nullptr;

        AcquireSRWLockShared(&m_lock);
        auto result = m_map.find(fd);
        handle = result != m_map.end() ? result->second : nullptr;
        ReleaseSRWLockShared(&m_lock);
        
        return handle;
    }

private:
    int GetNextHandle()
    {
        AcquireSRWLockShared(&m_lock);

        int fd = m_next;

        while (m_map.find(fd) != m_map.end())
        {
            fd = GetNextHandle(fd);
        }

        m_next = GetNextHandle(fd);

        ReleaseSRWLockShared(&m_lock);

        return fd;
    }

    static int GetNextHandle(int fd)
    {
        int next = fd + 1;
        return next > 0 ? next : 1;
    }

private:
    SRWLOCK m_lock = {};
    std::unordered_map<int, HANDLE> m_map;
    int m_next = 1;
};

Handles g_handles;

static int TranslateError(DWORD dwError = GetLastError())
{
    switch (dwError)
    {
    case ERROR_FILE_NOT_FOUND:
    case ERROR_PATH_NOT_FOUND:
        return ENOENT;
    case ERROR_ACCESS_DENIED:
        return EPERM;
    case ERROR_INVALID_HANDLE:
        return EINVAL;
    case ERROR_NOT_ENOUGH_MEMORY:
        return ENOMEM;
    default:
        return EIO;
    }
}

static void SetError(DWORD dwError = GetLastError())
{
    errno = TranslateError(dwError);
}

int usleep(unsigned int us)
{
    static MicroSleep microSleep;
    return microSleep.Sleep(us);
}

pid_t fork(void)
{
    return 0;
}

pid_t setsid(void)
{
    return 0;
}

int open(const char* pathname, int flags)
{
    DWORD dwDesiredAccess = 0;
    DWORD dwShareMode = 0;
    DWORD dwCreationDisposition = 0;
    DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED;
    switch (flags)
    {
    case O_RDWR:
        dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
        dwShareMode = 0;
        dwCreationDisposition = OPEN_EXISTING;
        break;
    default:
        errno = EINVAL;
        return -1;
    }

    std::string filename(pathname);
    if (_strnicmp(pathname, "COM", 3) == 0)
    {
        filename = "\\\\.\\" + filename;
    }

    auto handle = CreateFileA(
        filename.c_str(),
        dwDesiredAccess,
        dwShareMode,
        nullptr,
        dwCreationDisposition,
        dwFlagsAndAttributes,
        nullptr);
    if (handle == INVALID_HANDLE_VALUE)
    {
        SetError();
        return -1;
    }

    return g_handles.Add(handle);
}

int close(int fd)
{
    auto handle = g_handles.Lookup(fd);
    if (handle == 0)
    {
        errno = EINVAL;
        return -1;
    }

    CloseHandle(handle);
    g_handles.Remove(fd);
    return 0;
}

int fcntl(int fd, int cmd, ... /* arg */)
{
    return 0;
}

int select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, const struct timeval* timeout)
{
    return 0;
}

int poll(struct pollfd* fds, unsigned int nfds, int timeout)
{
    if (nfds != 1)
    {
        errno = EINVAL;
        return -1;
    }

    return 0;
}

ssize_t read(int fd, void* buffer, size_t count)
{
    auto handle = g_handles.Lookup(fd);
    if (handle == 0)
    {
        errno = EINVAL;
        return -1;
    }

    OVERLAPPED overlapped{};
    overlapped.hEvent = CreateEventW(nullptr, TRUE, FALSE, nullptr);

    DWORD dwBytesRead = 0;
    if (!ReadFile(handle, buffer, static_cast<DWORD>(count), &dwBytesRead, &overlapped))
    {
        DWORD dwError = GetLastError();
        if (dwError != ERROR_IO_PENDING)
        {
            CloseHandle(overlapped.hEvent);
            SetError(dwError);
            return -1;
        }
        else
        {
            if (!GetOverlappedResult(handle, &overlapped, &dwBytesRead, TRUE))
            {
                dwError = GetLastError();
                CloseHandle(overlapped.hEvent);
                SetError(dwError);
                return -1;
            }
            else
            {
                CloseHandle(overlapped.hEvent);
                return dwBytesRead;
            }
        }
    }
    else
    {
        CloseHandle(overlapped.hEvent);
        return dwBytesRead;
    }
}

ssize_t write(int fd, const void* buffer, size_t count)
{
    auto handle = g_handles.Lookup(fd);
    if (handle == 0)
    {
        errno = EINVAL;
        return -1;
    }

    OVERLAPPED overlapped{};
    overlapped.hEvent = CreateEventW(nullptr, TRUE, FALSE, nullptr);

    DWORD dwBytesWritten = 0;
    if (!WriteFile(handle, buffer, static_cast<DWORD>(count), &dwBytesWritten, &overlapped))
    {
        DWORD dwError = GetLastError();
        if (dwError != ERROR_IO_PENDING)
        {
            CloseHandle(overlapped.hEvent);
            SetError(dwError);
            return -1;
        }
        else
        {
            if (!GetOverlappedResult(handle, &overlapped, &dwBytesWritten, TRUE))
            {
                dwError = GetLastError();
                CloseHandle(overlapped.hEvent);
                SetError(dwError);
                return -1;
            }
            else
            {
                CloseHandle(overlapped.hEvent);
                return dwBytesWritten;
            }
        }
    }
    else
    {
        CloseHandle(overlapped.hEvent);
        return dwBytesWritten;
    }
}

int socketpair(int domain, int type, int protocol, int sv[2])
{
    if (domain == AF_UNIX && type == SOCK_STREAM)
    {
        static const char* name = "\\\\.\\pipe\\socketpair";

        auto hServer = CreateNamedPipeA(
            name,
            PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
            PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
            PIPE_UNLIMITED_INSTANCES,
            4096,
            4096,
            0,
            nullptr);
        if (hServer == INVALID_HANDLE_VALUE)
        {
            SetError();
            return -1;
        }

        auto hClient = CreateFileA(name,
            GENERIC_READ | GENERIC_WRITE,
            0,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
            nullptr);
        if (hClient == INVALID_HANDLE_VALUE)
        {
            CloseHandle(hServer);
            SetError();
            return -1;
        }

        DWORD dwMode = PIPE_READMODE_BYTE | PIPE_WAIT;
        SetNamedPipeHandleState(
            hClient,
            &dwMode,
            0,
            NULL);

        sv[0] = g_handles.Add(hServer);
        sv[1] = g_handles.Add(hClient);
        return 0;
    }
    else
    {
        errno = EINVAL;
        return -1;
    }
}
