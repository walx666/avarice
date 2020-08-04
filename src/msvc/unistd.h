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
#include <sys/socket.h>

#define O_RDWR 1
#define O_NOCTTY 0
#define O_NONBLOCK 0

#define	F_GETFL 3
#define	F_SETFL 4

typedef intptr_t ssize_t;
typedef int pid_t;

int usleep(unsigned int us);

pid_t fork(void);
pid_t setsid(void);


int open(const char* pathname, int flags);
int close(int fd);
int fcntl(int fd, int cmd, ... /* arg */);
int select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, const struct timeval* timeout);
int poll(struct pollfd* fds, unsigned int nfds, int timeout);
ssize_t read(int fd, void* buffer, size_t count);
ssize_t write(int fd, const void* buffer, size_t count);

int socketpair(int domain, int type, int protocol, int sv[2]);
