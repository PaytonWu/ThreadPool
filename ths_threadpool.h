#pragma once

namespace ths
{
namespace details
{
class threadpool_imp;
}

class task;
class threadpool
{
public:
    threadpool(LONG_PTR thread_count);
    ~threadpool(void);

    BOOL QueueTask(task* task, LPVOID task_resource);

private:
    details::threadpool_imp* _threadpool_imp;
};
}
