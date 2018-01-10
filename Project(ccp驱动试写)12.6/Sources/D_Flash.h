#include "includes.h"
#ifndef _D_Flash_H_
#define _D_Flash_H_

  /*  Flash��ʼ��״̬  */
  #define FlashInitOk         0x00
  #define FlashInitOverTime   0x01
  #define FlashSyncOverTime   0X02
  /*  DFlash��ַ���� */
  #define READword(address)     ((unsigned int)(*(volatile unsigned int *__near)(address)))
  #define READbyte(address)     ((unsigned int)(*(volatile unsigned char *__near)(address)))
  #define DFLASH_LOWEST_START_PAGE        0x00        //����data flash����ʼҳ
  #define DFLASH_START                    0x100000  //����data flash����ʼ��ַ
  #define DFLASH_PAGE_SIZE                0x0400      //����data flash�Ĵ�СΪ1K.
  #define DFLASH_PAGE_WINDOW_START        0x0800      //����data flashҳ�洰�ڵ���ʼ��ַ
  
  
  byte DFlash_Init(void);
  void DFlash_WriteBlock(byte SecNum,byte PieceNum,word a,word b,word c,word d);
  void DFlash_EraseSector(byte SecNum);
  byte DFlash_READbyte(byte Epage,byte SecNum,byte PieceNum,byte PosNum);

#endif