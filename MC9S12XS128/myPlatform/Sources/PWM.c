#include "PWM.h"


void InitPWM(void)     //ʹ�����໷��Ƶ�ʱ�Ϊ2kHZ,���ת��6000rpm
{
   PWMPOL_PPOL1   = 1;           //1ͨ���ȸߺ��
   PWMCLK_PCLK1   = 1;           //1ʹ��SAʱ��Դ
   PWMPRCLK_PCKA  = 0x04;        //Aʱ��Ϊ����16��Ƶ��clockA=32M/16=2M
   PWMCAE         = 0x00;        //���������
   PWMSCLA        = 0x04;        //Clock SA = Clock A / (2 * PWMSCLA) = 250KHZ
   PWMPER1        = 0x64;        //PWMxPeriod = PWMPERx / Clock SA = 100 / 250KHZ = 2.5KHZ
}


byte do_ctlDuty(char duty_t,byte dir)      
{
   if(duty_t > 100 || duty_t < 0)
      return 1;
   if(dir == SHIFT_DOWN)
   {
     MotorINA = Enable;
     MotorINB = Disable;
     PWMDTY1  = duty_t;        //��ȡռ�ձ�
     PWME_PWME1 = Enable;   //ʹ��1ͨ��
   } 
   else if(dir == SHIFT_UP)
   { 
     MotorINA = Disable;
     MotorINB = Enable;                                                                  
     PWMDTY1  = duty_t;        //��ȡռ�ձ�
     PWME_PWME1 = Enable;   //ʹ��1ͨ��
   }
   return 0;
}

void do_motorStop(void) 
{
   MotorINA = Disable;
   MotorINB = Disable;                                                                  
   PWMDTY1  = 0;        //��ȡռ�ձ�
   PWME_PWME1 = Disable;   //ʹ��1ͨ��  
}


