#include "SCI.h"

/**********************************************************************
*  ˵��:  ���ڽ����뷢����غ���
*        
*  ����:  Azmat
*  ����:  2017.12.15
***********************************************************************/

#define BAUD      9600 

//*sci0ģ���ʼ��
void InitSCI0(void) 
{
  SCI0BD = BUS_CLOCK/16/BAUD;   //����SCI0������Ϊ9600
  SCI0CR1 = 0x00;               //����SCI0Ϊ����ģʽ����λ����λ������żУ��
  SCI0CR2 = 0x2c;               //���������ݣ���������жϹ���
}
//*����һ���ֽ�����
static void sendSCI0(unsigned char data) 
{
  while(!SCI0SR1_TDRE);         //�ȴ��������ݼĴ�������������Ϊ��
  SCI0DRL = data;
} 
//*����һ���ֽ�����
byte receiveSCI0(void) 
{
  while(!SCI0SR1_RDRF);          //�ȴ��������ݼĴ�����
  return(SCI0DRL);
}

/**********************************************************************
*  ˵��:  �����ַ������ͺ���
*        
*  ����:  Azmat
*  ����:  2017.12.15
***********************************************************************/
void printk(char *str) 
{
  DisableInterrupts;
  while(*str != '\0')
      sendSCI0(*str++);
  EnableInterrupts;
}

/**********************************************************************
*  ˵��:  ���ڽ����жϺ����������غ���
*        
*  ����:  Azmat
*  ����:  2017.12.15
***********************************************************************/


typedef struct{
	byte head[3];
	byte num;
	byte cmd[8];
}uart_t;
uart_t uart;

static char uart_cmdacc,uart_msgcnt;
static byte *pcmd = uart.head;


#pragma CODE_SEG __NEAR_SEG NON_BANKED

void interrupt 20 receive_data(void)    //���ڽ���
{
  DisableInterrupts;
   
  	*pcmd++ = receiveSCI0();
		if(uart_cmdacc == 0)
			uart_msgcnt++;
		if(uart_msgcnt == 1 && uart.head[0] != 0xff){
			pcmd = uart.head;
			uart_msgcnt = 0;
		}
		if(uart_msgcnt == 2 && uart.head[1] != 0x5a){
			pcmd = uart.head;
			uart_msgcnt = 0;
		}
		if(uart_msgcnt == 3 && uart.head[2] != 0xa5){
			pcmd = uart.head;
			uart_msgcnt = 0;
		}
		
		if(uart_msgcnt == 4 && uart.num > 0){
		  if(uart.num > 8){
  		  //����ȴ���8�Ĳ�����Ӧ
  		  pcmd = uart.head;
  			uart_msgcnt = 0;
		  }else {
		    uart_cmdacc = 1;
		  	uart.num --; 
		  }
		}else if(uart_cmdacc == 1){
		  sciCmd_Decode();//����sci����
			pcmd = uart.head;
			uart_msgcnt = 0;
			uart_cmdacc = 0;
		}

  EnableInterrupts;
}


 #pragma CODE_SEG DEFAULT
 





