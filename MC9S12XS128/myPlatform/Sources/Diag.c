#include "Diag.h"
/**********************************************************************
*  ˵��:  ����Լ켰��������غ���
*        
*  ����:  Azmat
*  ����:  2017.1.8
***********************************************************************/

int ERRNO;   //������(�ؼ�����)


//*��ʼ����Ϻ���
byte early_selfDiagnose(void) 
{
  #if 0
  speed = readSpeed();  //��ȡת��
  adValue = atdChannel(1);  //��ȡadֵ
  if(speed < 0 || speed > 1000) {
    printk("please check speed sensor");
    return TCU_SENSORERR;
  }
  if(adValue < 100 || adValue > 900){
    printk("please check gear sensor");
    return TCU_SENSORERR;  
  }
  if(adVoltage > 1000){
    printk("battery voltage too high/low");
    return TCU_SENSORERR;
  }
  //if(CANACCESS)   can�����쳣(���ж�����ȡ)
    //return TCU_COMMUERR;
  #endif
  
  return TCU_NONEERR;
}

void reportError(int errno)
{
  if(errno)
    ;
}