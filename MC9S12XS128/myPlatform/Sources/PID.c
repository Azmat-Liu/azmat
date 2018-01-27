#include "PID.h"



/*************************************************************************
*  ˵��:  ������PI�㷨
*        
*  ����:  Azmat
*  ����:  2017.12.13
**************************************************************************/

struct PIDSING pid;
 
static int do_pidCalc(word GiveValue,word MeasureValue,struct PIDSING *pp)
{
   int dError,Error;
   int Uk,Duk;
   Error = GiveValue-MeasureValue;     // ƫ��
   dError = Error - pp->LastError;     // ��ǰ΢��
   Duk=pp->Proportion*dError+pp->Integral*Error;
   Uk=pp->lastUk + Duk/100;
   pp->LastError = Error;
   pp->lastUk = Uk;
   if(Uk>pp->PostivePIDLimit)
      Uk=pp->PostivePIDLimit;
   if(Uk<pp->NegativePIDLimit)
      Uk=pp->NegativePIDLimit;
   pp->lastUk=Uk;
   return Uk;
}
//*pid������ʼ��
void pidInit(void)
{
  pid.Proportion       = Trans.pid_p;
  pid.Integral         = Trans.pid_i;
  pid.Derivative       = Trans.pid_d;
  pid.LastError        = 0;
  pid.lastUk           = 0;
  pid.NegativePIDLimit = -1000;
  pid.PostivePIDLimit  = 1000;
}

//*����pid����ֵ
char pidCalc(word GiveValue,word MeasureValue) 
{
  int  pidValue;
  char rtnValue;
  
  pidValue = do_pidCalc(GiveValue,MeasureValue,&pid);
  rtnValue = (char)(pidValue/10); 
  
  if(rtnValue >= 0 && rtnValue < 20)
    rtnValue = 20;
  else if(rtnValue <=0 && rtnValue > -20)
    rtnValue = -20;
  return rtnValue;  
}

//����pid����
void pidSetPara(int P_t,int I_t,int D_t,int lastError,int lastUk,int n_limit,int p_limit)
{
  pid.Proportion       = P_t;
  pid.Integral         = I_t;
  pid.Derivative       = D_t;
  pid.LastError        = lastError;
  pid.lastUk           = lastUk;
  if(n_limit > -1000)
    pid.NegativePIDLimit = n_limit;
  else
    pid.NegativePIDLimit = -1000;
  if(p_limit < 1000)
    pid.PostivePIDLimit  = p_limit;
  else
    pid.PostivePIDLimit  = 1000;
}



















