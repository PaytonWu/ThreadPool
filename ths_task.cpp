#include "stdafx.h"
#include "ths_task.h"

namespace ths
{

task::~task(void)
{
}

UINT_PTR task::process(LPVOID param)
{
    UNREFERENCED_PARAMETER(param);
    return 0;
}

void task::postmortem(UINT_PTR return_value)
{
    UNREFERENCED_PARAMETER(return_value);
}

DWORD task::wait_for(DWORD dwMilliseconds) const
{
    return details::task_imp::wait_for(dwMilliseconds);
}

}   // namespace ths
