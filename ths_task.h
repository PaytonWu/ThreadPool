#pragma once

#include "ths_task_imp.h"

namespace ths
{

class task : public details::task_imp
{
public:
    virtual ~task(void);

    virtual UINT_PTR process(LPVOID param);
    virtual void postmortem(UINT_PTR return_value);

    DWORD wait_for(DWORD dwMilliseconds) const;
};
}
