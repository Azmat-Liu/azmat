#include "includes.h"
#ifndef _SCHEDULE_H_
#define _SCHEDULE_H_

  struct scheduleTable 
  {
	 byte rdystate;      //ָʾ�������״̬
	 word rdytaskTik[8]; //�����ʱ
  };
  extern struct scheduleTable Sch;
  
  extern void schUpdate(void);
  extern void rdyStateUpdate(void);
  
#endif 