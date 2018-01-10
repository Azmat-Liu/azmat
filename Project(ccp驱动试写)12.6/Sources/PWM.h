#include "includes.h"
#ifndef _PWM_H_                                                             
#define _PWM_H_

  #define Enable      1
  #define Disable     0 
  
  #define SHIFT_UP    0
  #define SHIFT_DOWN  1
  
  extern void InitPWM(void);
  extern byte ctlDuty(byte duty,byte dir);
  extern void motorStop(void);

#endif