#include "includes.h"
#ifndef _EXTCCP_H_
#define _EXTCCP_H_

  //������seed
  #define PL_CAL_SEED 0x0AE38E3A
  #define PL_PGM_SEED 0x0AEF8EA3
  #define PL_DAQ_SEED 0x0AE38E38
  
  //���ļ���������Ҫ�ⲿ���õĺ���������CCP.h�ļ�������
  extern CCP_BYTE ccpCalBuffer[256];  //����ccp�궨��Buffer��256���ֽ�
  extern CCP_BYTE ccpPROGRAMBuffer[256];   //����ccp����ˢ��Buffer��1024���ֽ�



#endif