#pragma once

namespace ths
{
namespace details
{

struct thread_context;

class thread_imp
{
public:
    thread_imp(void);
    ~thread_imp(void);

    LONG    QueuedTaskCount() const;
    BOOL    QueueTask(thread_context* task);

private:
    static DWORD WINAPI  ThreadProc(LPVOID);
    static void CALLBACK APCProc(ULONG_PTR);
    static void CALLBACK TaskProc(ULONG_PTR);

    void thread_proc();

private:
    HANDLE  _thread_stop_event;
    HANDLE  _thread_native_handle;
    mutable SRWLOCK _srwTaskCountLock;
    DWORD   _dwID;
    LONG    _queued_task_count;
};
}
}
