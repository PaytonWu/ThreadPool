#pragma once
namespace ths
{
class task
{
public:
    virtual ~task(void);

    virtual UINT_PTR process(LPVOID);
    virtual void postmortem(UINT_PTR);
};
}
