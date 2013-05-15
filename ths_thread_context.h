#pragma once

namespace ths
{
class task;

struct thread_context
{
    LPVOID  task_resource;
    task*   task;
    HWND    message_window;
    UINT    message;
};

}
