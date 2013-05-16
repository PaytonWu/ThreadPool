#include "stdafx.h"

#include <cassert>
#include <new>
#include <stdexcept>

#include "ths_task.h"
#include "ths_thread.h"
#include "ths_thread_context.h"

#include "ths_threadpool_imp.h"


namespace ths
{
namespace details
{

threadpool_imp::threadpool_imp(LONG_PTR thread_count)
    : _thread_count(thread_count) , _threads(0) , _thread_index(0)
{
    if (_thread_count <= 0)
    {
        throw std::invalid_argument("thread count is invalid");
    }

    _threads = new ths::thread[thread_count];
    if (NULL == _threads)
    {
        throw std::bad_alloc();
    }
}

threadpool_imp::~threadpool_imp()
{
    delete [] _threads;
    _threads = NULL;
    _thread_count = 0;
    _thread_index = 0;
}

BOOL threadpool_imp::QueueTask(ths::task* task, LPVOID task_resource)
{
    thread_context* context = NULL;
    try
    {
        context = new thread_context;
    }
    catch (std::bad_alloc&)
    {
        return FALSE;
    }

    if (NULL == context)
    {
        return FALSE;
    }

    context->task = task;
    context->task_resource = task_resource;

    return _threads[_thread_index++ % _thread_count].QueueTask(context);
}

}   // namespace details
}   // namespace ths
