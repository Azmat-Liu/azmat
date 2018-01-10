#include "includes.h"

/*************************************************************************
*  ˵��:  ����Ϊ:ccp��������չ����
*         ��ֲccp����ʱ��Ҫ���ڲ�ͬ����Ʒ����޸Ĵ���չ����
*  ����:  Azmat
*  ����:  2017.9.22
**************************************************************************/



/*************************************************************************
*  ˵��:  ccp���жϹرպ�ʹ��
*         �ֲ��ʹ���жϣ���ÿ�ر�һ���ж���Ҫ����һ�Σ������жϿ�����Ч
*  ����:  Azmat
*  ����:  2017.9.22
**************************************************************************/
CCP_BYTE disable_interrupt_count = 0;
void ccpDisableInterrupts(void) 
{
  _asm("SEI");                          // set I-Bit
  disable_interrupt_count++;            // incr. counter
}

void ccpEnableInterrupts(void) 
{
  if (--disable_interrupt_count==0) {   // if interrupts only one time disabled
    _asm("CLI");                        // enable them
  }
}

/*************************************************************************
*  ˵��:  ccp��ȡMTAָ��
*        
*  ����:  Azmat
*  ����:  2017.9.22
**************************************************************************/
/*��ȡȫ�ֵ�ַ��ָ��*/       
CCP_MTABYTEPTR ccpGetPointer( CCP_BYTE addr_ext, CCP_DWORD addr )
{ 
  addr_ext=0;  //��չ��ַλ����ʱ����       
  return (CCP_MTABYTEPTR) addr;
}

/*************************************************************************
*  ˵��:  ccp��ȡ��������
*        
*  ����:  Azmat
*  ����:  2017.9.22
**************************************************************************/
/*��ȡseed*/
CCP_DWORD ccpGetSeed( CCP_BYTE resourceMask )  //��ȡ����
{
  CCP_DWORD seed;
  switch(resourceMask)
  {
    case PL_CAL:  seed=PL_CAL_SEED;  break;
    case PL_PGM:  seed=PL_PGM_SEED;  break;
    case PL_DAQ:  seed=PL_DAQ_SEED;  break;
    default:
      break;
  }
  return (CCP_DWORD) seed; 
}
/*������Կ����,������Ӧ����*/
CCP_BYTE ccpUnlock( CCP_BYTE *key )       
{
  CCP_BYTE ProtectionStatus=0;
  CCP_BYTE i,KeyTable[6];
  CCP_DWORD KEY;      //���������Կ����
  for(i=0;i<6;i++)    //ȡ��Կ����
    KeyTable[i] = *((CCP_BYTE *)key+i);
  KEY = *((dword *)&KeyTable[0]);    
  //KEY *= 3;
  KEY &= 0xaa5a5aaa;
  //KEY /= 5;
  switch(KEY)
  {
    case PL_CAL_SEED:  ProtectionStatus=PL_CAL;  break;   //��ȡ��Ȩ��
    case PL_PGM_SEED:  ProtectionStatus=PL_PGM;  break;
    case PL_DAQ_SEED:  ProtectionStatus=PL_DAQ;  break;
    default:
      break;
  } 
  return ProtectionStatus;
}

/*************************************************************************
*  ˵��:  ccp�궨����д��DFlash��غ���
*        
*  ����:  Azmat
*  ����:  2017.9.22
**************************************************************************/

byte ccpCalBuffer[256]@0x3C00={1,2,3,4,5,6,7,8,9};       //����ccp�궨��Buffer��256���ֽ�
byte ccpPROGRAMBuffer[256]={0};  //����ccp����ˢ��Buffer��1024���ֽ�
byte ccpCalPage=0;                //����ccp�궨ҳ(��Ӧ������)
//�궨RAM�Ŀռ���ʼλ��
#define CCP_CAL_RAM_START ((CCP_DWORD)((CCP_MTABYTEPTR)&ccpCalBuffer[0]))    
#define CCP_CAL_RAM_END   (CCP_CAL_RAM_START+256)
/*����궨��ַ�Ƿ񳬳���Χ*/
CCP_BYTE ccpCheckWriteAccess( CCP_MTABYTEPTR a, CCP_BYTE s )
{
  CCP_DWORD address;
  address = (CCP_DWORD)a;
  if (address>CCP_CAL_RAM_START || (address+s)<CCP_CAL_RAM_END) 
  {
    return (TRUE);   //����д  
  } 
  else
    return (FALSE); //������д
}
/*��Buffer�е�����д��DFlash��*/
void cppCheckWriteDFlash(void)  
{ 
  byte i,calPage;                                        
  //�ֽ�������256ʱ�ɱ궨ҳ�����Ʋ�д��ַ
  DFlash_EraseSector(ccpCalPage);              //��������
  for(calPage=0;calPage<=ccpCalPage;calPage++) //�����ܱ궨ҳ������д����
  {
    for(i=0;i<32;i++)                          //д��������
    {
      DFlash_WriteBlock(calPage,i,
                        *(word *)&ccpCalBuffer[256*calPage+8*i],
                        *(word *)&ccpCalBuffer[256*calPage+8*i+2],
                        *(word *)&ccpCalBuffer[256*calPage+8*i+4],
                        *(word *)&ccpCalBuffer[256*calPage+8*i+6]
                        );  
    }
  }
}
/*************************************************************************
*  ˵��:  ccp�궨ҳ������غ���
*        
*  ����:  Azmat
*  ����:  2017.9.22
**************************************************************************/

void ccpInitCalPage( void )         //��ʼ���궨ҳ
{
  ccpCalPage=0;  
}

void ccpSetCalPage( CCP_DWORD a )  //���ñ궨ҳ(��0��ʼ���ϼ�)
{
  if(a>CCP_CAL_RAM_START)
    ccpCalPage = (a-CCP_CAL_RAM_START)%256;
}

CCP_DWORD ccpGetCalPage( void )   //��ȡʹ�õı궨ҳ
{
  return (CCP_DWORD)(ccpCalPage*256+CCP_CAL_RAM_START);     
}


/*************************************************************************
*  ˵��:  ccp���ͺ���(��CAN.c��ϵ����)
*        
*  ����:  Azmat
*  ����:  2017.9.22
**************************************************************************/

/*����CRM��DTM*/
void ccpSend( CCP_BYTEPTR msg )
{
  byte i;
  msg_send.id = CCP_DTO_ID; /* CAN identifier ECU -> Master */
  for(i=0;i<8;i++)
    msg_send.data[i] = *(msg+i);
  
  msg_send.len = CAN_DLC;
  msg_send.RTR = FALSE;
  msg_send.prty = 0;
  //����CRM/DTM
  if( MSCAN0SendMsg(msg_send) != CanTxOK )
    ccp.SendStatus |= CCP_TX_PENDING;
  else
    ccp.SendStatus &= ~CCP_TX_PENDING;
}

/*��ȡMTA[0]ָ���ȫ�ֵ�ַ(Ŀǰ������)*/
void ccpGetMTA0( CCP_BYTE *e, CCP_DWORD *a)   
{
  *e = 0;                      //��ַ��չΪ0(��ccpGetPointer��Ҫ��Ӧ)
  *a = (CCP_DWORD)ccp.MTA[0];  //����ccp.MTA[0]�ĵ�ַ
}




/*************************************************************************
*  ˵��:  ccp��̨�����������
*        
*  ����:  Azmat
*  ����:  2017.9.22
**************************************************************************/

/*�û������CCPBackground*/
byte Stamp1,Stamp2;
void ccpUserBackground(void)
{
  if(ccp.SendStatus & CCP_TX_PENDING)  //��CCP���ͱ����������·���
  {
    if( MSCAN0SendMsg(msg_send) == CanTxOK )
    {
      ccp.SendStatus &= ~CCP_TX_PENDING;
    }
    else
    {
      //��Ҫ���Ƿ��ͳɹ���᲻���η��͵�����,��������
      ccpSendCrm();   //���ͱ������Crm(�������ͱ�����ʱ,CCP����״̬��ΪCCP_CRM_REQUEST,��ccpSendCallBack���������·���)
      ccpSendDtm();   //ͬ��
    }
  }
  
  ccpDaq(1);  //Daq

  if(ccp.UserSessionStatus&SS_CAL)  //���û��Ự״̬��Ϊ�궨״̬��ʼ�궨
  {
    cppCheckWriteDFlash();  //��д�궨������DFlash
    memorycopy(ccpCalBuffer,&Trans,sizeof(Trans)); //��Buffer�е�����д��Trans
    ccp.UserSessionStatus &= ~SS_CAL; //�����־ 
  }
}





