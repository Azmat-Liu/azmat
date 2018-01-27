#include "D_Flash.h"



/******************************************************************/
/*       D_Flash Memory һ��8K                                    */
/*       ��Ƭ��Ԥ��һ��1K�ռ����Է���D_Flash                      */
/*       ��ַΪ:0x0800 - 0xBFF                                    */
/*       ͨ��EPAGE�Ĵ�����ȷ������һ����D_Flash���봰����         */
/******************************************************************/



/*************************************************************/
/*                      ��ʼ��DFLASH                         */
/*            DFLASH����Ե�ַ��ΧΪ0x0000-0x1fff            */
/*************************************************************/
byte InitDFlash(void)
{
  word timeCnt=0;
  while( (FSTAT_CCIF==0) && (timeCnt<5000))//�ȴ����ڴ����FLASH�������
    timeCnt++;  
  if( timeCnt>=5000)
    return (FlashInitOverTime);    //�ȴ���ʱ
  else timeCnt=0;             
  FCLKDIV=0x0F;                    //�ⲿ����Ϊ16M.FLASHʱ�Ӳ�����1M����������ֲ�
  FCNFG=0x00;                      //��ֹ�ж�
  while( (FCLKDIV_FDIVLD==0) && (timeCnt<500));        //�ȴ�ʱ�����óɹ�
    timeCnt++;  
  if( timeCnt>=500)
    return (FlashSyncOverTime);   //Flashͬ����ʱ
  else
    return (FlashInitOk);
}

/************************************************************/
/*    ���������źͿ�ź�λ�ã��������Ҫ�����ĵ�ַ          */
/*    ������secNum:�����ţ�0~31��                           */
/*    StartPos:��ţ�0~31��һ�������еĿ飬8�ֽ�һ��        */
/*    ���ص�ַ��ƫ�Ƶ�ַ��û��ȫ�ֵ�ַǰ׺0x10              */
/************************************************************/
word CallOffsetAddress(byte SecNum,byte PieceNum,byte PosNum) 
{
  //D_Flashÿ256bytesһ��������ÿ8bytesһ����
  unsigned int address;
  address = 0x0000 + 256*SecNum + 8*PieceNum + PosNum; 
  return (address);
}
dword CallGlobalAddress(byte SecNum,byte PieceNum,byte PosNum)   
{
  unsigned long address;
  address = 0x100000 + 256*SecNum + 8*PieceNum + PosNum; 
  return (address); 
}

/*************************************************************/
/*                    ����DFLASH��һ������                   */
/*            addressΪ�����������׵�ַ(ƫ�Ƶ�ַ)            */
/*************************************************************/
void DFlash_EraseSector(byte SecNum)
{ 
  unsigned int address;
  address = CallOffsetAddress(SecNum,0,0);  
  while(FSTAT_CCIF==0);
  if(FSTAT_ACCERR)           //�жϲ������־λ��
      FSTAT_ACCERR=1;
  if(FSTAT_FPVIOL)           //�жϲ������־λ��
      FSTAT_FPVIOL=1;
  
  FCCOBIX_CCOBIX=0x00;
  FCCOB=0x1210;           //д���������͸�λ��ַ
  FCCOBIX_CCOBIX=0x01;   
  FCCOB=address;          //д���16λ�ĵ�ַ
  FSTAT_CCIF=1;           //����ִ������
  while(FSTAT_CCIF==0);   //�ȴ�ִ�����
}

/*************************************************************/
/*                  ��DFlashд������(wordΪ��λ)             */
/*              addressΪд�����ݵ��׵�ַ(ƫ�Ƶ�ַ)          */
/*                  a��b��c��dΪд�������                   */
/*************************************************************/
void DFlash_WriteBlock(byte SecNum,byte PieceNum,word a,word b,word c,word d)
{
    unsigned int address;
    address = CallOffsetAddress(SecNum,PieceNum,0);  
    while(FSTAT_CCIF==0); 
    if(FSTAT_ACCERR)           //�жϲ������־λ��
        FSTAT_ACCERR=1;
    if(FSTAT_FPVIOL)           //�жϲ������־λ��
        FSTAT_FPVIOL=1;
    FCCOBIX_CCOBIX=0x00; 
    FCCOB=0x1110;         //д������͸�λ��ַ��д�������ڸ�8λ����8λΪ��λ��ַ���˴�Ϊ16Byte
    FCCOBIX_CCOBIX=0x01;  //��ַ��16λ
    FCCOB=address;         //д���16λ��ַ
    FCCOBIX_CCOBIX=0x02;  //д���һ������
    FCCOB=a;
    FCCOBIX_CCOBIX=0x03;  //д��ڶ�������
    FCCOB=b;
    FCCOBIX_CCOBIX=0x04;  //д�����������
    FCCOB=c;
    FCCOBIX_CCOBIX=0x05;  //д����ĸ�����
    FCCOB=d;
      
    FSTAT_CCIF=1;         //д��ִ������
    while(FSTAT_CCIF==0); //�ȴ�ִ�����
}

/*************************************************************/
/*              ��DFLASH��ȡ����(byteΪ��λ)                 */
/*              addressΪ��ȡ���ݵ�ȫ�ֵ�ַ                  */
/*************************************************************/
byte DFlash_READbyte(byte Epage,byte SecNum,byte PieceNum,byte PosNum)   //���ֽڶ�ȡ
{
  byte lastepage;    //���ڴ洢EPAGE��ֵ
  word address;
  byte data;
  
  address = CallOffsetAddress(SecNum,PieceNum,PosNum);
  lastepage = EPAGE;   //����EPAGE��ֵ
  EPAGE=Epage;  //��EPAGE��ֵ
  data = READbyte(DFLASH_PAGE_WINDOW_START + address);  //��ȡҳ�洰���е�����
  EPAGE= lastepage;       //�ָ�EPAGE��ֵ
  
  return (data);
}
word DFlash_READword(byte Epage,word index)   //���ֶ�ȡ(�����������������飬�����ڴ����е����λ�ö�λ)
{
  byte lastepage;    //���ڴ洢EPAGE��ֵ
  word data;
  
  lastepage = EPAGE;   //����EPAGE��ֵ
  EPAGE=Epage;  //��EPAGE��ֵ
  data = READword(DFLASH_PAGE_WINDOW_START + index);  //��ȡҳ�洰���е�����
  EPAGE= lastepage;       //�ָ�EPAGE��ֵ


  return(data);
}





























