#include "stdafx.h"

#include <cassert>
#include <exception>
#include <stdexcept>

#include "ths_task.h"
#include "ths_thread.h"
#include "ths_thread_context.h"

#include "ths_threadpool_imp.h"

///////////////////////////////////////////////////////////////////////////////
// Message definition
#define TASK_DONE_NOTIFY_MSG WM_USER

///////////////////////////////////////////////////////////////////////////////
// static
WNDPROC ths::details::threadpool_imp::_prev_message_only_wnd_proc = 0;

namespace ths
{
namespace details
{
    threadpool_imp::threadpool_imp(LONG_PTR thread_count)
        : _thread_count(thread_count) , _threads(0) , _thread_index(0)
        , _message_wnd(0)
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

        if (!ConstructMessageDispatcherWnd())
        {
            delete [] _threads;
            throw std::runtime_error("message dispatch window failed");
        }
    }

    threadpool_imp::~threadpool_imp()
    {
        delete [] _threads;
        DestroyWindow(_message_wnd);

        _threads = NULL;
        _message_wnd = NULL;
        _thread_count = 0;
        _thread_index = 0;
    }

    BOOL threadpool_imp::QueueTask(ths::task* task, LPVOID task_resource)
    {
        ths::thread_context* context = NULL;
        try
        {
            context = new ths::thread_context;
        }
        catch (std::bad_alloc&)
        {
            return FALSE;
        }

        if (NULL == context)
        {
            return FALSE;
        }

        context->message = TASK_DONE_NOTIFY_MSG;
        context->message_window = _message_wnd;
        context->task = task;
        context->task_resource = task_resource;

        return _threads[_thread_index++ % _thread_count].QueueTask(context);
    }

    BOOL threadpool_imp::ConstructMessageDispatcherWnd()
    {
        _message_wnd = ::CreateWindowEx(0, _T("Message"), NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, 0, NULL, NULL);
        if (!IsWindow(_message_wnd))
        {
            return FALSE;
        }

        _prev_message_only_wnd_proc =
            reinterpret_cast<WNDPROC>(SetWindowLongPtr(_message_wnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(ths_message_only_wnd_proc)));
        if (0 == _prev_message_only_wnd_proc)
        {
            DestroyWindow(_message_wnd);
            _message_wnd = NULL;

            return FALSE;
        }

        return TRUE;
    }

    LRESULT threadpool_imp::ths_message_only_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        UINT_PTR task_process_return_value;
        ths::thread_context* context;

        switch (uMsg)
        {
        case TASK_DONE_NOTIFY_MSG:
            task_process_return_value = static_cast<UINT_PTR>(wParam);
            context = reinterpret_cast<ths::thread_context*>(lParam);
            assert(context);

            context->task->postmortem(task_process_return_value);

            delete context;

            break;

        default:
            return CallWindowProc(_prev_message_only_wnd_proc, hWnd, uMsg, wParam, lParam);
        }

        return 0;
    }
}
}
