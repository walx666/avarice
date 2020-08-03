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

#include "socket.h"
#pragma comment(lib, "ws2_32.lib")

class Sockets
{
public:
    Sockets()
    {
        if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) == 0)
        {
            m_success = true;
        }
    }

    ~Sockets()
    {
        if (m_success)
        {
            WSACleanup();
        }
    }

private:
    WSADATA m_wsaData = { 0 };
    bool m_success = false;
};

static Sockets sockets;

static int TranslateSocketError(int error = WSAGetLastError())
{
    switch (error)
    {
    case WSAEINVAL:
    case WSA_INVALID_PARAMETER:
        return EINVAL;
    case WSA_NOT_ENOUGH_MEMORY:
        return ENOMEM;
    case WSAENETDOWN:
    case WSAENETUNREACH:
    case WSAENETRESET:
    case WSAECONNABORTED:
        return EPIPE;
    default:
        return EINVAL;
    }
}

static void SetSocketError(int error = WSAGetLastError())
{
    errno = TranslateSocketError(error);
}

int socketpair(int domain, int type, int protocol, int sv[2])
{
    auto socket0 = WSASocket(domain, type, protocol, NULL, 0, 0);
    if (socket0 == INVALID_SOCKET)
    {
        SetSocketError();
        return -1;
    }

    sv[0] = socket0;
    return 0;
}
