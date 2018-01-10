#include "ECT.h"




struct IC_Parameters_Tag Pca;   //���벶׽ֵ���� 

void InitECT(void)
{
   PERT=0xff;           //ʹ��������
   PPST=0x00;           //ʹ����������
   DDRT=0xfe;           //����PORTT�ķ���Ĵ���
   
   TSCR2=0x87;          //���÷�Ƶ����Ϊ128 ,fECT=250KHZ,����Ϊ4us,��ʱ������ж�����
   TIOS=0xfe;           //TC0��TC2����Ϊ���벶׽
   TCTL4=0x01;          //TC0��TC2�����ش���
   TFLG1=0xff;	        // �����IC/OC�жϱ�־λ
   
   TFLG2_TOF=1;         //�����־λ��0
   TIE=0x01;            //TC0��TC2�ж�����
   TSCR1=0x80;          //TEN=1����־λ�������
   TCTL2_OM1=0;
   TCTL2_OL1=0;         //����1���������
   CFORC_FOC1=1;        //ǿ������Ƚ�
   OC7M_OC7M1=0;
   TTOV_TOV1=0;         //��ֹ�������
}

/***********************************************************/
/*                   ��ƽ��ת�ٺ���                        */
/***********************************************************/
dword aveValue=0;
//dword Sum[5],Sum2;
word readSpeed(void) 
{
  byte i,j;
  word rtnValue;
  dword Max,Min;//,aveValue=0;
  static dword Sum[5],Sum2;  //�˴���Ҫ����Ϊ���߷����ڴ���������������(ԭ����)
  if(Pca.IC0_Buffer[IC0_BUFFERSIZE] != 0)
  {
    for(i=0;i<3;i++)
    {
      Max = Pca.IC0_Buffer[6*i];    //������֮ǰ��MAX��MIN��ʼ����������Ŵ���һ�εļ���ֵ���������
      Min = Pca.IC0_Buffer[6*i];
      for(j=0;j<6;j++)
      {
        Sum[i] += Pca.IC0_Buffer[6*i+j];
        if(Max <= Pca.IC0_Buffer[6*i+j])
          Max = Pca.IC0_Buffer[6*i+j];
        else if(Min >= Pca.IC0_Buffer[6*i+j])
          Min = Pca.IC0_Buffer[6*i+j];
      } 
      Sum2 += (Sum[i]-Max-Min)/4;
      Sum[i] = 0;  
    }
    aveValue = Sum2/3;
    rtnValue = (word)(15000000/(8*aveValue)); 
    Sum2 = 0;
  }else {
    rtnValue = 0;  
  }

  return (rtnValue);
}  
  
  
/***********************************************************/
/*                    ECT�жϷ�����                      */
/***********************************************************/

#pragma CODE_SEG __NEAR_SEG NON_BANKED

  /*         TC0���벶׽�ɼ����������ת��         */
  void interrupt 8  TC0_Capture(void)
  {
     static index=0;
     TFLG1_C0F = 1;
     Pca.IC0Counter = TC0;                          //��õ�ǰIC0�ļ���ֵ
     Pca.IC0 = Pca.IC0Counter + Pca.OverFlowCount*65536 - Pca.IC0LastCounter;       //�õ�ǰ����ֵ���ϸ��ݼ���ֵ�õ�����ֵ��
     if(Pca.IC0 >=250000)
        Pca.IC0 = 250000;
     if(Pca.IC0 <=250)
        Pca.IC0 = 250;
     Pca.OverFlowCount = 0;
     Pca.IC0_StopFlag = 0;
     Pca.IC0LastCounter = Pca.IC0Counter;     //��ǰ����ֵ��Ϊ�ϸ��ݼ���ֵ���棬�´�ʹ��
     index++;                            //����ֵ��������Լӣ����ڼ���n����ƽ��ת��
     if(index >= IC0_BUFFERSIZE)                
        index = 0;                       //���������n��ƽ��ת�٣����ٵ�20�����
     Pca.IC0_Buffer[index] = Pca.IC0;    //�洢����������Լ���ƽ��ת��
  }

  void interrupt 16 TimeOverFlow(void)      //����ж�
  {
      byte i;
      TFLG2_TOF = 1;  //���־
      Pca.OverFlowCount++;
      if(Pca.OverFlowCount >= 6)     //1.5sû�������򴢴�375000
      {
        Pca.IC0_StopFlag = 1;
        for(i=0;i<IC0_BUFFERSIZE;i++)
          Pca.IC0_Buffer[i] = 250000;  
      }
  }

#pragma CODE_SEG DEFAULT

   
