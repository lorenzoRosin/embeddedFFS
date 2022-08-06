#include "ParamOrganizer.h"

int main(void);

int main(void)
{
    s_paramOrgContext prmCntx;
    s_paramOrgInitParameter prmInitVal;
    e_paramOrgResult result;
    result = initParamSettings(&prmCntx, &prmInitVal);
    return 0;
}
