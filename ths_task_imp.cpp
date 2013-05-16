#include "stdafx.h"

#include <stdexcept>

#include "ths_task_imp.h"

namespace ths
{
namespace details
{

task_imp::task_imp(void)
    : _task_done_event(::CreateEvent(NULL, TRUE, FALSE, NULL))
{
    if (NULL == _task_done_event)
    {
        throw std::runtime_error("task notify event creation failed");
    }
}

task_imp::~task_imp(void)
{
    ::CloseHandle(_task_done_event);
}

void task_imp::task_done(UINT_PTR return_value)
{
    postmortem(return_value);
    ::SetEvent(_task_done_event);
}

DWORD task_imp::wait_for(DWORD dwMilliseconds) const
{
    return ::WaitForSingleObject(_task_done_event, dwMilliseconds);
}

}   // namespace details
}   // namespace ths
