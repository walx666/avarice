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

#include "termios.h"

int cfsetispeed(struct termios* termiosp, speed_t speed)
{
    return -1;
}

int cfsetospeed(struct termios* termiosp, speed_t speed)
{
    return -1;
}

int tcsetattr(int fd, int options, const struct termios* termiosp)
{
    return -1;
}

int tcdrain(int fd)
{
    return -1;
}

int tcflush(int fd, int cmd)
{
    return -1;
}

int tcgetattr(int fd, struct termios* termiosp)
{
    return -1;
}
