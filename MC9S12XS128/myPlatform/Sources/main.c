#include "includes.h"


word heartbeat;  //��������ʱѭ�����
  
/***********************************************************
*                   main�������������壩                 
***********************************************************/
void main(void) 
{
	
	ERRNO=sysInit(); //ϵͳ��ʼ��
	
	for(;;)
	{
	  _FEED_COP();       //ι��
	  ERRNO = early_selfDiagnose();   //�����Լ�
	  reportError(ERRNO);             //���󱨸�
	  while(ERRNO == TCU_NONEERR)     //��ϵͳ����
	  {
	    heartbeat++;
      _FEED_COP();     //ι��
      
      schUpdate();     //�����������
	  }
	}
    /*
    ccp.DaqList[0].odt[0][0].ptr = (CCP_DAQBYTEPTR)&aaaa;
    bbbb = *ccp.DaqList[0].odt[0][0].ptr; 
    */
}  
