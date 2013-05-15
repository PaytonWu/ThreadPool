 一个基于Windows APC的轻量级线程池。

 class task是线程执行的基本单位。task::process是线程的执行体，它类似Windows里的ThreadProc(http://msdn.microsoft.com/en-us/library/windows/desktop/ms686736%28v=vs.85%29.aspx)
 task::postortem是process执行完成后通知处理函数。该函数只有一个参数，是process的函数返回值。

 task通过线程池(threadpool)对象的QueueTask指派给一个指定的线程。目前的调度算法就是按续轮询指派。