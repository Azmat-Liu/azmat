#include "includes.h"
#ifndef _SYSINIT_H_
#define _SYSINIT_H_

  #define BUS_CLOCK		   32000000	   //����Ƶ��
  #define OSC_CLOCK		   16000000	   //����Ƶ��
  //canоƬ�˿�
  #define CANSlope           PTP_PTP0  
  #define CANSlope_Dir       DDRP_DDRP0
  //�������оƬ�˿�
  #define MotorINA_Dir       DDRA_DDRA7   
  #define MotorINB_Dir       DDRA_DDRA3
  #define MotorINA           PORTA_PA7 
  #define MotorINB           PORTA_PA3
extern byte sysInit(void);
  
#endif 