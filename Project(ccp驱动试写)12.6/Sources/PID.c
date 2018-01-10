#include "PID.h"



/**********************************************/
/*              ����PID�ṹ��                 */
/**********************************************/

struct PIDSING pid;
 
int pidCalc(word GiveValue,word MeasureValue,struct PIDSING *pp)
{
 long dError,Error;
 long Uk,Duk;
 Error = GiveValue-MeasureValue;     // ƫ��
 dError = Error - pp->LastError;     // ��ǰ΢��
 Duk=pp->Proportion*dError+pp->Integral*Error;
 Uk=pp->lastUk+Duk;
 pp->LastError = Error;
 pp->lastUk=Uk;
 if(Uk>pp->PostivePIDLimit)
    Uk=pp->PostivePIDLimit;
 if(Uk<pp->NegativePIDLimit)
    Uk=pp->NegativePIDLimit;
 pp->lastUk=Uk;
 return Uk;
}








