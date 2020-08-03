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
#include <stdint.h>

#define IGNPAR      (1 << 4)

#define CS8         (3 << 0)
#define CREAD       (1 << 3)
#define CLOCAL      (1 << 7)

#define VEOF        0
#define VMIN        VEOF
#define VEOL        1
#define VTIME       VEOL

#define B9600       9600
#define B19200      19200
#define B38400      38400
#define B57600      57600
#define B115200     115200

#define TCSANOW     0
#define TCIFLUSH    0

typedef uint32_t speed_t;
typedef uint16_t tcflag_t;
typedef int cc_t;

struct termios {
    tcflag_t c_iflag;
    tcflag_t c_oflag;
    tcflag_t c_cflag;
    tcflag_t c_lflag;
    cc_t c_cc[32];
};

int cfsetispeed(struct termios* termiosp, speed_t speed);
int cfsetospeed(struct termios* termiosp, speed_t speed);
int tcsetattr(int fd, int options, const struct termios* termiosp);
int tcdrain(int fd);
int tcflush(int fd, int cmd);
int tcgetattr(int fd, struct termios* termiosp);
