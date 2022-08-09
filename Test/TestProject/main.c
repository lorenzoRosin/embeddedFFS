#include "eFSS.h"

int main(void);

int main(void)
{
    s_eFSS_Ctx prmCntx;
    s_eFSS_InitParam prmInitVal;
    e_eFSS_Res result;
    result = initParamSettings(&prmCntx, &prmInitVal);
    return 0;
}
