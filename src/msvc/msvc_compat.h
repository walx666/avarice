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

#pragma once
#include <io.h>
#include <string.h>
#include <sys/socket.h>

#define SIGHUP 1
#define SIGTRAP 5

#define O_NOCTTY 0
#define O_NONBLOCK 0

typedef size_t ssize_t;

static inline int strcasecmp(const char* s1, const char* s2) { return _stricmp(s1, s2); }
static inline int strncasecmp(const char* s1, const char* s2, size_t n) { return _strnicmp(s1, s2, n); }

static inline int ffs(int value)
{
    if (value == 0)
        return 0;

    int bit = 1;
    while ((value & 1) == 0)
    {
        value >>= 1;
        bit++;
    }

    return bit;
}
