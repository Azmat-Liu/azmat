#include "SysInit.h"

/**********************************************************************
*  ˵��:  ϵͳ��ʼ����غ���
*        
*  ����:  Azmat
*  ����:  2017.12.15
***********************************************************************/

//*��ʼ�����໷
static void InitPLL(void)           //����ʱ��fbus=32M
{
    CLKSEL &= 0x7f;       //����ʱ��=OSCCLK/2
    PLLCTL &= 0x8F;       //��ֹPLL
    CRGINT &= 0xDF;
    
    #if(BUS_CLOCK == 40000000) 
      SYNR = 0x44;
    #elif(BUS_CLOCK == 32000000)
      SYNR = 0x43;     
    #elif(BUS_CLOCK == 24000000)
      SYNR = 0x42;
    #endif 

    REFDV = 0x81;         //PLLCLK=2��OSCCLK��(SYNDIV+1)/(REFDIV+1)��64MHz ,fbus=32M
    PLLCTL =PLLCTL|0x70;  //Enable PLL circuit
    asm NOP;
    asm NOP;
    while(!(CRGFLG&0x08)); //PLLCLK is Locked already
    CLKSEL |= 0x80;        //set PLLCLK as sysclk
}

/***********************************************************************
*  ˵��:  �弶����ģ���ʼ����غ���
*        
*  ����:  Azmat
*  ����:  2017.12.3
************************************************************************/
static byte modulesInit(void)
{
  
   InitPLL();
   (void)InitDFlash();
   (void)InitMSCAN0();
   InitSCI0();
   InitPIT();
   InitECT();
   InitATD();
   InitPWM();
  
   return 0;
}
/***********************************************************************
*  ˵��:  GPIO�˿�����
*        
*  ����:  Azmat
*  ����:  2017.12.3
************************************************************************/

static void gpioInit(void)
{
  //canоƬ����
  CANSlope_Dir  = 1;
  CANSlope      = 0;
  //�������оƬ�˿�����
  MotorINA_Dir   = 1;
  MotorINB_Dir   = 1;
    
  MotorINA   = Disable;
  MotorINB   = Disable;
  
}

/***********************************************************************
*  ˵��:  �û�������������ʼ��
*        
*  ����:  Azmat
*  ����:  2017.12.3
************************************************************************/
static void dataInits(void)  //������ʼ��
{
    word i;
    for(i=0;i<IC0_BUFFERSIZE;i++)     //��ʼ��ectBuffer
          Pca.IC0_Buffer[i] = 250000; 
    adTarget = 200;
}

static void userInit(void)
{
  ccpInit();         //��ʼ��CCP�ṹ��
  ccpInitCalPage();  //��ʼ���궨ҳ
  ccpDaqOdtInit();   //��ʼ��DAQ�б�
  DFlashmemorycopy(DFLASH_START,(byte *)&ccpCalBuffer,sizeof(ccpCalBuffer));  //0x10 0000ΪDFlash�Ļ�ַ
  pidInit();         //��ʼ��PID����
  dataInits();
}

/**********************************************************************
*  ˵��:  ϵͳ��ʼ��������
*        
*  ����:  Azmat
*  ����:  2017.12.22
***********************************************************************/

byte sysInit(void)
{
  DisableInterrupts;
    _DISABLE_COP();    //�رտ��Ź�
    
    if(modulesInit())
      return TCU_SYSINITERR;   //ģ���ʼ������
    gpioInit();
    userInit();
    
    _ENABLE_COP_X();   //ʹ�ܿ��Ź�
  EnableInterrupts;
  
  printk("System Initialization Finished !\n");    //��ӡ��Ϣ
  
  return TCU_NONEERR;
}


