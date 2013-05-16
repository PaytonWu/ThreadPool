#include "stdafx.h"

#include <new>

#include "ths_thread_imp.h"
#include "ths_thread.h"

namespace ths
{
thread::thread(void)
    : _thread_imp(0)
{
    _thread_imp = new details::thread_imp;
    if (NULL == _thread_imp)
    {
        throw std::bad_alloc();
    }
}

thread::~thread(void)
{
    delete _thread_imp;
    _thread_imp = NULL;
}

BOOL thread::QueueTask(details::thread_context* task)
{
    return _thread_imp->QueueTask(task);
}

}   // namespace ths
