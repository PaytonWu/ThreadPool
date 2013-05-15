#include "stdafx.h"

#include <exception>

#include "ths_threadpool_imp.h"
#include "ths_threadpool.h"

namespace ths
{
threadpool::threadpool(LONG_PTR thread_count)
    : _threadpool_imp(0)
{
    _threadpool_imp = new details::threadpool_imp(thread_count);
    if (NULL == _threadpool_imp)
    {
        throw std::bad_alloc();
    }
}

threadpool::~threadpool(void)
{
    delete _threadpool_imp;
    _threadpool_imp = NULL;
}

BOOL threadpool::QueueTask(task* task, LPVOID task_resource)
{
    return _threadpool_imp->QueueTask(task, task_resource);
}

}
