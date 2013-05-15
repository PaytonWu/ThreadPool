#pragma once

namespace ths
{
class task;
class thread;

namespace details
{
class threadpool_imp
{
public:
    threadpool_imp(LONG_PTR thread_count);
    ~threadpool_imp();

//    DWORD   QueueUserAPC(HWND notify_wnd, UINT notify_msg, PAPCFUNC fpnAPC, ULONG_PTR parameter);
    BOOL    QueueTask(ths::task* task, LPVOID task_resource);

private:
    BOOL ConstructMessageDispatcherWnd();
    static LRESULT CALLBACK ths_message_only_wnd_proc(HWND, UINT, WPARAM, LPARAM);

private:
    HWND         _message_wnd;
    LONG_PTR     _thread_count;
    ths::thread* _threads;
    ULONG_PTR    _thread_index;

    static WNDPROC     _prev_message_only_wnd_proc;
};

}
}