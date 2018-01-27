#include "ATD.h"


void InitATD(void) 
{
  ATD0CTL1=0x20;  //10λת������
  ATD0CTL2=0x40;  //�Խ���Ĵ����ķ��ʽ��Զ������ӦCCF��־λ������������
  ATD0CTL3=0x88;  //ÿ��ֻת��1��ͨ��,��FIFOģʽ,����ģʽ�¼���ת��
  ATD0CTL4=0x01;  //����ʱ��Ϊ4��ʱ�����ڣ�prs=1;ATDclock=BusClock/((prs+1)*2)
  ATD0DIEN=0x00;   // ��ֹ��������
}

word atdChannel(byte ch) 
{
 uchar count,i;
 int Max,Min,a[5],Sum,rtnValue;  
 switch(ch)
 { 
  case 1:
 
     for(count=0;count<5;count++) 
       {
        ATD0CTL5 = 0x01;    //ת��AD01
        while(!ATD0STAT0_SCF);//�ж�һ��ת���������
        a[count]=ATD0DR0;
       }
     Max=a[0];
     Min=a[0];
     Sum=a[0];
     for(i=1;i<5;i++) 
       {
        Sum += a[i];
        if(a[i]>Max) Max=a[i];
        if(a[i]<Min) Min=a[i];
       }
     rtnValue=(Sum-Max-Min)/3;      
     break; 

  case 2:
    
    for(count=0;count<5;count++) 
       {
        ATD0CTL5 = 0x02;    //ת��AD02
        while(!ATD0STAT0_SCF);//�ж�һ��ת���������
        a[count]=ATD0DR0;
       }
     Max=a[0];
     Min=a[0];
     Sum=a[0];
     for(i=1;i<5;i++) 
       {
        Sum += a[i];
        if(a[i]>Max) Max=a[i];
        if(a[i]<Min) Min=a[i];
       }
     rtnValue=(Sum-Max-Min)/3;      
     break;
     
  case 3:
    
    for(count=0;count<5;count++) 
       {
        ATD0CTL5 = 0x03;    //ת��AD03
        while(!ATD0STAT0_SCF);//�ж�һ��ת���������
        a[count]=ATD0DR0;
       }
     Max=a[0];
     Min=a[0];
     Sum=a[0];
     for(i=1;i<5;i++) 
       {
        Sum += a[i];
        if(a[i]>Max) Max=a[i];
        if(a[i]<Min) Min=a[i];
       }
     rtnValue=(Sum-Max-Min)/3;      
     break;
  
 }
 
 return rtnValue;

}







