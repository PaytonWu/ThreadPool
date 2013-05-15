#include "stdafx.h"

#include "ths_task.h"

namespace ths
{
task::~task(void)
{
}

UINT_PTR task::process(LPVOID)
{
    return 0;
}

void task::postmortem(UINT_PTR)
{
}

}
