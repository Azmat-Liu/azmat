#include "Schedule.h"

/**********************************************************************
*  ˵��:  ���������غ���
*        
*  ����:  Azmat
*  ����:  2017.12.15
***********************************************************************/
#define PROCESS0  0
#define PROCESS1  2
#define PROCESS2  3
#define PROCESS3  4
#define PROCESS4  5
#define PROCESS5  6
#define PROCESS6  7
#define PROCESS7  8

#define CanCmdTxPeriod   10     //CAN���������(����������й�)

 
static byte taskstartTable[8]=
{
  PROCESS0,PROCESS1,PROCESS2,PROCESS3,
  PROCESS4,PROCESS5,PROCESS6,PROCESS7
};//��������ƿ�ʼ������ʱ��(��֤Ϊ������)

static word taskperiodTable[8]=
{
  CanCmdTxPeriod,10,10,2,2,10,10,10
};//�������������ִ�е�����

//���������������
static byte startmaskTable[8]=
{
  0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80
};//�����������Щ����ʼ����(���ɸ���)
static byte taskrdyTable[8]=
{
  0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80
};//����������ָʾ��Щ�������(���ɸ���)

/***********************************************************************
*  ˵��:  �����������״̬������ֵ������λ����PIT�ж���
*         ���ȷ�ʽΪ�����Ե��ȡ��ɸ���taskstartTable�е�ֵ
          ����������1��8�Ŀ�ʼʱ�̣���ͨ������taskperiodTable
          �е�ֵ����������1��8�ľ������ڡ�
*        
*  ����:  Azmat
*  ����:  2017.12.3
************************************************************************/
struct scheduleTable Sch; //�������״̬�ṹ��
void rdyStateUpdate(void) 
{
   byte i; 
   static word Tik;
   static byte startState;
   //��ʼ״̬   
   if(Tik<=8)           //��ѯtaskstartTable����������ʼʱ��
   {
     for(i=0;i<8;i++)
     {
       if(Tik == taskstartTable[i])
         startState |= startmaskTable[i];  
     }
     Tik++;             //�����˸�����Ͳ��ٿ�ʼ����������
   }
   //�ֱ��ۼ�
   for(i=0;i<8;i++)     //��������Ŀ�ʼ״̬�ֱ��ۼ�
   {
      if(startState&startmaskTable[i])
        Sch.rdytaskTik[i]++;
   }
   //��ʱ���¾���״̬
   for(i=0;i<8;i++)     //��������Ŀ�ʼ״̬�ֱ��ۼ�
   {
      if(Sch.rdytaskTik[i] >= taskperiodTable[i])
        Sch.rdystate |= taskrdyTable[i];
   } 
}


/***********************************************************************
*  ˵��:  ������������񣬿�ֱ�ӽ�����������Ӧ�ľ�������
*        
*  ����:  Azmat
*  ����:  2017.12.3
************************************************************************/

void schUpdate(void) 
{
  byte schedule;
  schedule = Sch.rdystate;  //��ȡ����״̬
  if(schedule & 0x01)  //���ڷ���CAN����
  {
    //add you functions here
    CANMsg_Update();
    //Sch.rdytaskTik[0] = 0;����CAN�ķ����жϴ��ڹ�����Դ�
    Sch.rdystate &= ~(0x01); //�������״̬
  } //�������
  if(schedule & 0x02)  //���ڼ��BUS_OFF״̬
  {
    //add you functions here
    BusOff_Update();
    Sch.rdytaskTik[1] = 0;
    Sch.rdystate &= ~(0x02);
  }
  if(schedule & 0x04)
  {
    //add you functions here
    speed = readSpeed();  //��ȡת��
    adValue = atdChannel(1);  //��ȡadֵ
    Sch.rdytaskTik[2] = 0;
    Sch.rdystate &= ~(0x04);
  }
  if(schedule & 0x08)
  {
    //add you functions here
    //adTarget = ccpCalBuffer[0]*256 + ccpCalBuffer[1];
   
    (byte)motorCtl(adTarget,adValue,30);   //�����������
    Sch.rdytaskTik[3] = 0;
    Sch.rdystate &= ~(0x08);
  }
  if(schedule & 0x10)
  {
    //add you functions here
    Sch.rdytaskTik[4] = 0;
    Sch.rdystate &= ~(0x10);
  }
  if(schedule & 0x20)
  {
    //add you functions here
    Sch.rdytaskTik[5] = 0;
    //Sch.rdystate &= ~(0x20);
  }
  if(schedule & 0x40)
  {
    //add you functions here
    Sch.rdytaskTik[6] = 0;
    Sch.rdystate &= ~(0x40);
  }
  if(schedule & 0x80)
  {
    //add you functions here
    (void)ccpBackground();       //ccp����ά������
    Sch.rdytaskTik[7] = 0;
    Sch.rdystate &= ~(0x80);
  }
  
}