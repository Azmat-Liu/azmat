#include "DAQ.h"
/*************************************************************************
*  ˵��:  ccpDAQ��غ���
*        
*  ����:  Azmat
*  ����:  2017.9.22
**************************************************************************/
/*��дDAQ List��ODT List*/
byte x0=0,x1=1,x2=2,x3=3,x4=4,x5=5,x6=6,x7=7;
void ccpDaqOdtInit(void) 
{
  	ccp.DaqList[0].odt[0][0].ptr = (CCP_DAQBYTEPTR)&x0;
  	ccp.DaqList[0].odt[0][0].ptr = (CCP_DAQBYTEPTR)&x1;
  	ccp.DaqList[0].odt[0][0].ptr = (CCP_DAQBYTEPTR)&x2;
  	ccp.DaqList[0].odt[0][0].ptr = (CCP_DAQBYTEPTR)&x3;
  	ccp.DaqList[0].odt[0][0].ptr = (CCP_DAQBYTEPTR)&x4;
  	ccp.DaqList[0].odt[0][0].ptr = (CCP_DAQBYTEPTR)&x5;
  	ccp.DaqList[0].odt[0][0].ptr = (CCP_DAQBYTEPTR)&x6;
  	ccp.DaqList[0].odt[0][0].ptr = (CCP_DAQBYTEPTR)&x7;
}