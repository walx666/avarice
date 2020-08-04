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

#include "pthread.h"
#include <process.h>

struct ThreadContext
{
    using start_routine_t = void* (*)(void*);
    start_routine_t start_routine = nullptr;
    void* arguments = nullptr;
};

static thread_local const ThreadContext* g_context;

static void pthread_start_routine_wrapper(void* arguments)
{
    auto context = static_cast<const ThreadContext*>(arguments);
    if (g_context == nullptr)
    {
        g_context = context;
    }

    context->start_routine(context->arguments);
}

int pthread_create(pthread_t* thread, const void* attr, void* (*start_routine) (void*), void* arg)
{
    auto context = new ThreadContext();
    context->start_routine = start_routine;
    context->arguments = arg;

    uintptr_t handle = _beginthread(pthread_start_routine_wrapper, 0, context);
    if (handle == static_cast<uintptr_t>(-1))
    {
        delete context;
        return -1;
    }

    *thread = reinterpret_cast<pthread_t>(handle);
    return 0;
}

void pthread_exit(void* retval)
{
    delete g_context;
    g_context = nullptr;

    _endthreadex(static_cast<unsigned>(reinterpret_cast<uintptr_t>(retval)));
}
