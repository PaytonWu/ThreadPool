#pragma once

namespace ths
{
namespace details
{

struct thread_context
{
    LPVOID      task_resource;
    ths::task*  task;
    //HWND        message_window;
    //UINT        message;
};

}
}
