#include "stdafx.h"

#include <stdexcept>
#include <cassert>

#include "ths_task.h"
#include "ths_thread_context.h"

#include "ths_thread_imp.h"

namespace ths
{
namespace details
{
thread_imp::thread_imp(void)
    : _thread_stop_event(0) , _thread_native_handle(0)
    , _dwID(0) , _queued_task_count(0)
{
    _thread_stop_event = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (NULL == _thread_stop_event)
    {
        throw std::runtime_error("thread stop event cration failed");
    }

    _thread_native_handle = CreateThread(NULL, 0, ThreadProc, this, 0, &_dwID);
    if (NULL == _thread_native_handle)
    {
        CloseHandle(_thread_stop_event);
        throw std::runtime_error("thread creating failed");
    }

    InitializeSRWLock(&_srwTaskCountLock);
}

thread_imp::~thread_imp(void)
{
    SetEvent(_thread_stop_event);
    if (WAIT_OBJECT_0 == WaitForSingleObject(_thread_native_handle, INFINITE))
    {
        CloseHandle(_thread_native_handle);
    }
    CloseHandle(_thread_stop_event);
}

DWORD WINAPI thread_imp::ThreadProc(LPVOID param)
{
    thread_imp* pthis = static_cast<thread_imp*>(param);
    pthis->thread_proc();
    return 0;
}

#pragma warning(disable:4127)
void thread_imp::thread_proc()
{
    while (true)    // warning C4127: conditional expression is constant
    {
        if (WAIT_OBJECT_0 == WaitForSingleObjectEx(_thread_stop_event, INFINITE, TRUE))
        {
            return;
        }
    }
}
#pragma warning(default:4127)

void thread_imp::TaskProc(ULONG_PTR parameter)
{
    thread_context* context = reinterpret_cast<thread_context*>(parameter);
    assert(context);

    task* task = context->task;
    assert(task);

    task->task_done(task->process(context->task_resource));
}

BOOL thread_imp::QueueTask(thread_context* task)
{
    return static_cast<BOOL>(::QueueUserAPC(TaskProc, _thread_native_handle, reinterpret_cast<ULONG_PTR>(task)));
}

LONG thread_imp::QueuedTaskCount() const
{
    AcquireSRWLockShared(&_srwTaskCountLock);
    LONG count = _queued_task_count;
    ReleaseSRWLockShared(&_srwTaskCountLock);

    return count;
}

}   // namespace details
}   // namespace ths
