#include "eFSS.h"
#include "Prv_eFSSUtilsTest.h"

int main(void);

int main(void)
{
    bool_t testResult;

    testResult = Prv_eFSSUtilsTestFunc();

    if( false == testResult )
    {
        while(true)
        {
            uint32_t error;
            error++;
        }
    }

    return 0;
}
