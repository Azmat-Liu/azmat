#include "MotorCtl.h"

/**********************************************************************
*  ˵��:  ����Զ����ƺ���������Ŀ��ֵ������ֵ�Ϳ��ƾ���
*         ����ʵ�ֶԵ�����Զ�����
*        
*  ����:  Azmat
*  ����:  2017.12.15
***********************************************************************/
int pidError;   //ƫ��
char duty;
byte motorCtl(word target,word measure,byte accuracy)
{
  char range;
  if(accuracy > 100 || accuracy < 0){
    printk("out of range \n");
    return 1;
  }
  range = (char)accuracy;   //����ǿ��ת��Ϊ�з��ŵ����ͷ���ȽϵĽ������ȷ
  pidError = target - measure; 
  
  if(pidError > range || pidError < -range){
    duty = pidCalc(target,measure);
    
    if(duty >= 0){
      if(do_ctlDuty(duty,SHIFT_UP))
        printk("duty out of range \n");
    }
    else{
      if(do_ctlDuty(-duty,SHIFT_DOWN))
        printk("duty out of range \n");
    }
  }else{
    do_motorStop();
    pid.lastUk = 0;
    pid.LastError = 0;
  }
  return 0;
}