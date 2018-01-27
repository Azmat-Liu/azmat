#include "includes.h"
#ifndef _CAN_H_
#define _CAN_H_

  /*CAN ��������*/
  #define CAN_BOUND_RATE      500    //CAN���߲�����500kbps
  #define CAN_Filters_Enable  0      //ѡ���Ƿ�ʹ��CAN�����˲���,1Ϊʹ��,0Ϊ��ʹ��
  #define CAN_DLC             8      //�������ݳ���
  //#define CanCmdTxPeriod      10     //CAN���������(����������й�)
  /*CAN ��ʼ��״̬*/
  #define CanInitOK                   0x00
  #define CanEnterInitModeOverTime    0x01
  #define CanEnterNormalModeOverTime  0x02
  #define CanSyncOverTime             0x04
  #define CanNotInSleepMode           0x08
  /*CAN ����״̬*/
  #define CanTxOK          0x01
  #define CanTxFull        0x02
  #define CanTxLenOverRun  0x04
  #define CanTxNotSync     0x08
  #define CanTxErrIDLen    0x10
  /*CAN ����״̬*/
  #define CanBusNormal    0x01
  #define CanBusWarning   0x02
  #define CanBusError     0x03
  #define CanBusOff       0x04
  //CAN����ID
  #define CAN0_RX_First_ID     0x403   //CAN0���յ�һ֡ID   
  #define CAN0_RX_Second_ID    0x469   //CAN0���յڶ�֡ID 
  #define CAN0_TX_ID     0x10F     //CAN0����ID
  //��������λ��ͨ��
  //#define CCP_DTO_ID        0x10E           /* CAN identifier ECU -> Master */
  //#define CCP_CRO_ID        0x10D           /* CAN identifier Master -> ECU */
  //CAN���Ľṹ 
  struct CAN_Messages       
  {
    unsigned long id;
    Bool RTR;
    unsigned char data[8];
    unsigned char len;
    unsigned char prty;
  };
  extern struct CAN_Messages msg_send, msg_get;

  static byte CANSetFilter(void);
  static void CANSetParams(void);
  static byte CANEnterSleepMode(void);
  static byte CANExitSleepMode(void);
  static byte CANGetTransceiverState(void);
  static byte CANGetTxBufferState(void);
  static byte CANBusOffRecover(void);
  byte InitMSCAN0(void);
  void BusOff_Update(void);
  void CANMsg_Update(void);
  byte MSCAN0SendMsg(struct CAN_Messages msg);
  byte MSCAN0GetMsg(struct CAN_Messages *msg);
  void canMessageWrite(void);
  


#endif
