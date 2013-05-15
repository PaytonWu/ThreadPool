#pragma once
namespace ths
{
namespace details
{
    class thread_imp;
}

struct thread_context;

class thread
{
public:
    thread(void);
    ~thread(void);

    BOOL QueueTask(ths::thread_context* task);

private:
    details::thread_imp* _thread_imp;
};
}
