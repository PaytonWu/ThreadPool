#pragma once
namespace ths
{
namespace details
{

class task_imp
{
public:
    task_imp(void);
    virtual ~task_imp(void);

    virtual UINT_PTR process(LPVOID) = 0;
    virtual void postmortem(UINT_PTR return_value) = 0;

    void task_done(UINT_PTR return_value);

    DWORD wait_for(DWORD dwMilliseconds) const;

private:
    HANDLE  _task_done_event;
};

}   // namespace details
}   // namespace ths
