#include "CAN.h"


struct CAN_Messages msg_send,msg_get;    //CAN���߷��ͽṹ

/*********************************************/
/*                CAN0��ʼ��                 */
/*********************************************/

byte InitMSCAN0(void)      //��ʼ��MSCAN
{
  word timeCnt=0;
  if(CAN0CTL0_INITRQ==0)      // ��ѯ�Ƿ�����ʼ��״̬   
    CAN0CTL0_INITRQ =1;        // ��������ʼ��״̬
  //�ȴ������ʼ��״̬
  while ((CAN0CTL1_INITAK==0) && (timeCnt<500))
    timeCnt++;  
  if( timeCnt>=500 )
    return (CanEnterInitModeOverTime);
  else timeCnt=0; 
  //ʹ��MSCANģ��,����Ϊһ������ģʽ��ʹ������ʱ��Դ
  CAN0CTL1_CANE   = 1; //����MSCANģ��
  CAN0CTL1_CLKSRC = 1; //ʹ������ʱ��
  CAN0CTL1_LOOPB  = 0; //��ѭ���Լ�ģʽ
  CAN0CTL1_LISTEN = 0; //�Ǽ���ģʽ
  CAN0CTL1_BORM   = 1; //�û��ɻָ�Bus-Off
  CAN0CTL1_WUPM   = 0; //���������Ա�������ʱ��������
  /*******************************************
  *   �������ú��� :   
  *   ��Ҫ��CANE=1�������
  *   �ſ����ã��������������Ч  
  *********************************************/
  CANSetParams(); //����CAN ����
  (void)CANSetFilter();  //�����˲���
  
  //���󷵻�һ��ģʽ����              
  CAN0CTL0 = 0x00;             
  //�ȴ��ص�һ������ģʽ 
  while( (CAN0CTL1_INITAK) && (timeCnt <500) ) 
    timeCnt++;        
  if( timeCnt>=500 )
    return (CanEnterNormalModeOverTime);
  else timeCnt=0;
  //�ȴ�����ʱ��ͬ��
  while( (CAN0CTL0_SYNCH==0) && (timeCnt <500) )
    timeCnt++;    
  if( timeCnt>=500 )
    return (CanSyncOverTime);
  //�ж�ʹ��
  CAN0RIER_RXFIE = 1;          //ʹ�ܽ��ջ��������ж�
  //CAN0RIER_CSCIE = 1;          //ʹ��CAN����״̬�ı��ж�
  //CAN0RIER_RSTATE = 1;         //01 �����յ�����������ʱ����CSCIE�ж�
  //CAN0RIER_TSTATE = 1;         //01 �����͵�����������ʱ����CSCIE�ж�
  return (CanInitOK);
}

/*********************************************/
/*              CAN0��������               */
/*********************************************/

static void CANSetParams(void) //����CAN���߲���
{
  CAN0BTR0_SJW = 1;      //ͬ����Ծ�������Ϊ1
  CAN0BTR0_BRP = 7;            //���ò����� 
  #ifndef CAN_BOUND_RATE
    #error "Please define CAN_BOUND_RATE"
  #endif 
  #if(CAN_BOUND_RATE == 500) 
    CAN0BTR1 = 0x14;      //����ʱ��1��ʱ��2��Tq���� ,����Ƶ��Ϊ500kb/s
  #else if(CAN_BOUND_RATE == 250)
    CAN0BTR1 = 0x1c;      //����ʱ��1��ʱ��2��Tq���� ,����Ƶ��Ϊ250kb/s
  #endif 
  
}

/*********************************************/
/*              CAN0�˲�������               */
/*********************************************/

static byte CANSetFilter(void)   //�����˲���
{
    CAN0IDAC_IDAM = 0x01;   //bit4= 1��bit5= 0��Four 16-bit acceptance filters
  #if(CAN_Filters_Enable == 1)     //ʹ��CAN�����˲���
    //����ȫ���˲���
    CAN0IDMR0 = 0x02; //����
    CAN0IDMR1 = 0x60;
    CAN0IDMR2 = 0x00;
    CAN0IDMR3 = 0x00;
    CAN0IDMR4 = 0x00;
    CAN0IDMR5 = 0x00;
    CAN0IDMR6 = 0x00;
    CAN0IDMR7 = 0x00;
    //����ӦID�����˲�
    //��һ֡
    CAN0IDAR0 = (byte)(CAN0_RX_First_ID>>3);
    CAN0IDAR1 = (byte)(CAN0_RX_First_ID<<5);
    CAN0IDAR2 = 0x29;
    CAN0IDAR3 = 0x40;
    //�ڶ�֡
    CAN0IDAR4 = (byte)(CAN0_RX_Second_ID>>3);
    CAN0IDAR5 = (byte)(CAN0_RX_Second_ID<<5);
    CAN0IDAR6 = (byte)(CCP_CRO_ID>>3);
    CAN0IDAR7 = (byte)(CCP_CRO_ID<<5);
    
  #else if(CAN_Filters_Enable == 0)     //��ʹ��CAN�����˲���
    //�ر������˲���
    CAN0IDMR0 = 0xFF;
    CAN0IDMR1 = 0xFF;
    CAN0IDMR2 = 0xFF;
    CAN0IDMR3 = 0xFF;
    CAN0IDMR4 = 0xFF;
    CAN0IDMR5 = 0xFF;
    CAN0IDMR6 = 0xFF;
    CAN0IDMR7 = 0xFF;
  #endif
  #if( CAN_Filters_Enable != 1 && CAN_Filters_Enable != 0 )
    #error "Please define CAN_Filters_Enable corretly"
    return (FALSE);  
  #endif
  return (TRUE);
}
/*********************************************
*  CAN0����˯��ģʽ(���ɻ���״̬)                   
*********************************************/
static byte CANEnterSleepMode(void)
{
  word timeCnt=0;
  //�������˯��ģʽ
  CAN0CTL0_SLPRQ = 1;
  DisableInterrupts;
    /*  ����������  */
    CAN0CTL0_WUPE  = 0;
    CAN0RIER_WUPIE = 0;  //��ֹ�����ж�
    while( (CAN0CTL1_SLPAK == 0) && (timeCnt <1000) )
      timeCnt++; 
  EnableInterrupts;   
  if( timeCnt>=1000 )
    return (FALSE);
  else 
    return (TRUE);
}
/*********************************************
*  CAN0�˳�˯��ģʽ                   
*********************************************/
static byte CANExitSleepMode(void)
{
  word timeCnt=0;
  //�������˯��ģʽ
  CAN0CTL0_SLPRQ = 0;
  /*  ����������  */
  CAN0CTL0_WUPE  = 0;
  CAN0RIER_WUPIE = 0;  //��ֹ�����ж�
  while( (CAN0CTL1_SLPAK == 1) && (timeCnt <1000) )
    timeCnt++;    
  if( timeCnt>=1000 )
    return (FALSE);
  else 
    return (TRUE);  

}

/*********************************************/
/*                CAN0����                   */
/*********************************************/

byte MSCAN0SendMsg(struct CAN_Messages msg)
{
  unsigned char send_buf, sp;
  if(msg.len > 8)        // ������ݳ���
    return(CanTxLenOverRun);
  if(CAN0CTL0_SYNCH==0)  // �������ʱ��
    return(CanTxNotSync);
  if(!(CAN0TFLG&7))
    return (CanTxFull);  //����������
  // Ѱ�ҿ��еĻ�����
  CAN0TBSEL_TX=CAN0TFLG;
  send_buf=CAN0TBSEL;
  if(msg.id <= 0x7FF)
  {  // ��׼֡
    CAN0TXIDR0 = (unsigned char)(msg.id>>3);
    CAN0TXIDR1 = (unsigned char)(msg.id<<5);
  } 
  else if(msg.id > 0x7FF && msg.id <= 0x1FFFFFFF)
  { //  ��չ֡          
    CAN0TXIDR0 = (msg.id>>21) & 0xFF;         
    CAN0TXIDR1 = (msg.id>>18) & 0x07; 
    CAN0TXIDR1 = CAN0TXIDR1 << 5;
    CAN0TXIDR1 = CAN0TXIDR1 | ((msg.id>>15) & 0x07);
    CAN0TXIDR1_SRR = 1;
    CAN0TXIDR1_IDE = 1;                            //��չ֡ 
    CAN0TXIDR2 = ( msg.id>>7) & 0xFF; 
    CAN0TXIDR3 = ( msg.id<<1) & 0xFF; 
    CAN0TXIDR3_RTR = 0; 
  } 
  else
    return (CanTxErrIDLen);  //ID����
  if(msg.RTR)
    CAN0TXIDR1 |= 0x10;       // RTR = ����
  for(sp = 0; sp < msg.len; sp++)
    *((&CAN0TXDSR0)+sp) = msg.data[sp];    // д������
    
  CAN0TXDLR_DLC = msg.len;  // д�����ݳ���
  // д�����ȼ�
  CAN0TXTBPR = msg.prty;
  // �� TXx ��־ (������׼������)
  CAN0TFLG = send_buf;
  //�򿪶�Ӧ�ķ��ͻ��������ж�
  CAN0TIER_TXEIE = send_buf;       
  
  return(CanTxOK);
}
/*********************************************/
/*                CAN0����                  */
/********************************************/

byte MSCAN0GetMsg(struct CAN_Messages *msg)
{
  unsigned char sp2;
  dword idreg;
  // �����ձ�־
  if(!(CAN0RFLG_RXF))
    return(FALSE);
  // ��� CANЭ�鱨��ģʽ ��һ��/��չ�� ��ʶ��
  if(CAN0RXIDR1_IDE)    //��չ��ʶ��
  {
    idreg= *((dword*)((dword)(&CAN0RXIDR0)));
    msg->id=((idreg>>1)& 0x3FFFF)|((idreg>>3)& 0x1FFC0000);    //��չ֡ģʽ��ȡ֡ID
    
    if(CAN0RXIDR3&0x01)
      msg->RTR = TRUE;
    else
      msg->RTR = FALSE;
  }
  else      //��׼֡
  {
    msg->id = (unsigned int)(CAN0RXIDR0<<3) |  (unsigned char)(CAN0RXIDR1>>5);
              
    if(CAN0RXIDR1&0x10)
      msg->RTR = TRUE;
    else
      msg->RTR = FALSE;
  }
  // ��ȡ���ݳ��� 
  msg->len = CAN0RXDLR_DLC;
  // ��ȡ����
  for(sp2 = 0; sp2 < msg->len; sp2++)
    msg->data[sp2] = *((&CAN0RXDSR0)+sp2);
  // �� RXF ��־λ (������׼������)
  CAN0RFLG = 0x01;

  return(TRUE);
}

/*********************************************/
/*               CAN���ϻָ�                 */
/*********************************************/
static byte CANBusOffRecover(void)
{
  byte value = 0;
  if(CANEnterSleepMode())
  {
  DisableInterrupts;
    CAN0MISC_BOHOLD = 1;     //���Bus-Off��־
    value = InitMSCAN0();    //���³�ʼ��
  EnableInterrupts;
  }
  else
    value = CanNotInSleepMode;
  return (value);
}
//��ȡCAN������״̬
word canBusWarningCount=0;    //����ת��Ϊ�����ʱ�������BusOff_Update�ĵ��������й�
static byte CANGetTransceiverState(void) 
{
  byte value = 0;
  if( CAN0RFLG_RSTAT == 0 && CAN0RFLG_TSTAT == 0)
    value = CanBusNormal;
  else if( CAN0RFLG_RSTAT == 1 || CAN0RFLG_TSTAT == 1) 
  {
    canBusWarningCount++;
    value = CanBusWarning;
    if(canBusWarningCount >= 500)    //���������̫�࣬����Ϊ�Ǵ���
      value = CanBusError;
  }
  else if( CAN0RFLG_RSTAT == 2 || CAN0RFLG_TSTAT == 2)
    value = CanBusError;
  else if( CAN0RFLG_RSTAT == 3 || CAN0RFLG_TSTAT == 3)
    value = CanBusOff;
  return (value);
}
//��ȡ���ͻ�����״̬
static byte CANGetTxBufferState(void)
{
  byte value;
  value = CAN0TFLG_TXE;
  
  return (value);
}
/*********************************************************************
* ���� :CANBud-Offˢ�º���
* ���� :��֤���ͻ������������
*       ���CAN����״̬,���ڹ���ʱ��ʱһ��ʱ���ָ�
* ���� :2017.9.20
*********************************************************************/
word canBusOffTimeCnt;   //BusOff�ָ���ʱ
void BusOff_Update(void)
{
  if( (CANGetTransceiverState() == CanBusError) || (CANGetTransceiverState() == CanBusOff))
  {
    //PITDelay(50);  //�������ʱ
    canBusOffTimeCnt++;
    if( canBusOffTimeCnt >= 5000)
      (void) CANBusOffRecover(); 
  }
  else
    canBusOffTimeCnt = 0;
}
/*********************************************************************
* ���� :CAN���ڷ��ͺ���
* ���� :���ڷ��������
* ���� :2017.9.21
*********************************************************************/
void CANMsg_Update(void)
{
   canMessageWrite();
   (void)MSCAN0SendMsg(msg_send);

}

/***********************************************************/
/*                   CANͨѶ���ݴ��                       */
/***********************************************************/
void canMessageWrite(void)   //��дCAN��������
{
    msg_send.id = CAN0_TX_ID;
    msg_send.data[0] += 1;
    msg_send.data[1] += 2;
    msg_send.data[2] = 3;
    msg_send.data[3] += 4; 
    msg_send.data[4] = 5;    //Logic_Speed/256;
    msg_send.data[5] = 6;    //Logic_Speed%256;
    msg_send.data[6] = 10;
    msg_send.data[7] = 1;
    msg_send.len = CAN_DLC;
    msg_send.RTR = FALSE;
    msg_send.prty = 0;
      
}
/*
*����Э�����
*/
byte TPS,Battery_Soc,Motor_Mode;
word Motor_Speed,Contorller_Torque;  
void protocol(dword id) 
{
  word temp1,temp2,temp3;
  switch(id)
  {
    case   CAN0_RX_First_ID:
        
       temp1 = 100*msg_get.data[6];
       TPS = (byte)(temp1/255);        //���ſ��� % 
       break;
    case   CAN0_RX_Second_ID:
                                            
       temp2 = msg_get.data[4]*256 + msg_get.data[5];    
       Battery_Soc = (byte)(temp2/100);    //��ص��� %     
       break;
    case   CAN0_RX_Third_ID:
                                                
       temp3 = msg_get.data[2]*256 + msg_get.data[3];        
       Motor_Speed = temp3/2-11998;               //���ת��
       Contorller_Torque = msg_get.data[4]*256 + msg_get.data[5];   //ʵ�����Ť��
       break;
    case   CAN0_RX_Fourth_ID:
   
       Motor_Mode = msg_get.data[4]&0x7c;       //���ģʽ(ȡ�м�5λ��Ϊ�ж�����)     
       break;
                                             
     default:     
        break;               
  }
}

/***********************************************************/
/*                   CAN0�����жϷ�����                  */
/***********************************************************/
char canRX_error,canTX_error;

#pragma CODE_SEG __NEAR_SEG NON_BANKED
//�����ж�
void interrupt 38 CAN_receive(void) 
{
    if( MSCAN0GetMsg(&msg_get)&&(!msg_get.RTR) )   //����Э�����
    {  
      switch(msg_get.id)
      {
         case   CCP_CRO_ID:   //������ն���ID
           {
              ccpCommand(msg_get.data);   //��������׵�ַ����ccpCommand
           }
           break;

                                         
         default:     
              protocol(msg_get.id);
                 
       }  
     } 
 }
 
 //�����ж�
void interrupt 39 CAN_transmit(void)
{
  CAN0TIER_TXEIE = 0x00; //��ֹ���з��ͻ��������ж�
  Sch.rdytaskTik[0] = 0;    //����ʱ��
  if( CANGetTxBufferState() != 0)
  {
    /*��ExtCCP��ϵĺ������޷�����*/
    (void)ccpSendCallBack();
  }
}
#pragma CODE_SEG DEFAULT


















