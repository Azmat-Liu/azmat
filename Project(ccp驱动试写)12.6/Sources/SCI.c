#include "SCI.h"


#define BUS_CLOCK 32000000 
#define BAUD      9600 



void InitSCI0(void) 
{
  SCI0BD = BUS_CLOCK/16/BAUD;   //����SCI0������Ϊ9600
  SCI0CR1 = 0x00;               //����SCI0Ϊ����ģʽ����λ����λ������żУ��
  SCI0CR2 = 0x2c;               //���������ݣ���������жϹ���
}

static void sendSCI0(unsigned char data) 
{
  while(!SCI0SR1_TDRE);         //�ȴ��������ݼĴ�������������Ϊ��
  SCI0DRL = data;
} 


static byte receiveSCI0(void) 
{
  while(!SCI0SR1_RDRF);          //�ȴ��������ݼĴ�����
  return(SCI0DRL);
}


void printk(char *str) 
{
  DisableInterrupts;
  while(*str != '\0')
      sendSCI0(*str++);
  EnableInterrupts;
}

/***********************************************************/
/*                     ����ͨѶ�����ж�                    */
/***********************************************************/
byte SCI0receivebuff[33];
#pragma CODE_SEG __NEAR_SEG NON_BANKED

  interrupt void receive_data(void)    //���ڽ���
  {
      unsigned char i;
      for(i=0;i<33;i++)          
        SCI0receivebuff[i] = receiveSCI0(); 
      //sciCommand(); 
  }


 #pragma CODE_SEG DEFAULT
 
 void sciCommand(void)
 {
    ;  //sci�����д
 }





