#include "ParamOrganizer.h"

int main(void);

int main(void)
{
    s_paramOrgContext prmCntx;
    e_paramOrgResult result;
    result = initParamSettings(&prmCntx, 256u, 2u, 0u, PARAMORGAN_BKPPAGE_EN | PARAMORGAN_RAW_MODE);
    return 0;
}
