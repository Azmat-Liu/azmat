#include "includes.h"
#ifndef _PID_H_
#define _PID_H_


  struct PIDSING  
  {
   int  Proportion;         //  �������� Proportional Const
   int  Integral;           //  ���ֳ��� Integral Const
   int  Derivative;         //  ΢�ֳ��� Derivative Const
   dword  LastError;
   dword  lastUk;
   int    NegativePIDLimit;  
   int    PostivePIDLimit; 
  };
  
#endif                             