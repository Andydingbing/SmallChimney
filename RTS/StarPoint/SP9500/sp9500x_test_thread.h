#ifndef Q_TEST_SP9501_THREAD_H
#define Q_TEST_SP9501_THREAD_H

#include "q_winthread.h"

namespace NS_SP9500X {

struct Test_Baseband_Param : TestBaseParam
{
};

class Q_Test_Baseband_Thread : public QTestThread
{
public:
    Q_Test_Baseband_Thread(Test_Baseband_Param *param) : QTestThread(param) {}

    void run();
    void runDMA();

private:
    Test_Baseband_Param *param;
};

} // namespace NS_SP9500X

#endif // Q_TEST_SP9501_THREAD_H
