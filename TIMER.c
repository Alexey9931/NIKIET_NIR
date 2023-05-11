#include "TIMER.h"


TIMER_CntInitTypeDef TIMERInitStruct;
TIMER_ChnInitTypeDef sTIM_ChnInit;
TIMER_ChnOutInitTypeDef sTIM_ChnOutInit;

//инициализация 1-го таймера (период 3 сек)
void TIMER1_init(void)
{
	 RST_CLK_PCLKcmd (RST_CLK_PCLK_TIMER1, ENABLE);
	 /* Initializes the TIMERx Counter */
	 TIMER_CntStructInit(&TIMERInitStruct);
	 TIMERInitStruct.TIMER_Period = 0xF424;//62500
	 TIMERInitStruct.TIMER_Prescaler = 0x180;//384
	 TIMER_CntInit(MDR_TIMER1, &TIMERInitStruct);
	
	
	 /* Initializes the TIMER1 Channel 1,1N,2,2N,3 */
   TIMER_ChnStructInit(&sTIM_ChnInit);
   sTIM_ChnInit.TIMER_CH_Mode       = TIMER_CH_MODE_PWM;
   sTIM_ChnInit.TIMER_CH_REF_Format = TIMER_CH_REF_Format6;
   sTIM_ChnInit.TIMER_CH_Number     = TIMER_CHANNEL1;
   TIMER_ChnInit(MDR_TIMER1, &sTIM_ChnInit);

   TIMER_SetChnCompare(MDR_TIMER1, TIMER_CHANNEL1, 62499);

    /* Initializes the TIMER1 Channel 1,1N,2,2N,3 Output */
   TIMER_ChnOutStructInit(&sTIM_ChnOutInit);

   sTIM_ChnOutInit.TIMER_CH_DirOut_Polarity = TIMER_CHOPolarity_NonInverted;
   sTIM_ChnOutInit.TIMER_CH_DirOut_Source   = TIMER_CH_OutSrc_REF;
   sTIM_ChnOutInit.TIMER_CH_DirOut_Mode     = TIMER_CH_OutMode_Output;
   sTIM_ChnOutInit.TIMER_CH_NegOut_Polarity = TIMER_CHOPolarity_NonInverted;
   sTIM_ChnOutInit.TIMER_CH_NegOut_Source   = TIMER_CH_OutSrc_REF;
   sTIM_ChnOutInit.TIMER_CH_NegOut_Mode     = TIMER_CH_OutMode_Output;
   sTIM_ChnOutInit.TIMER_CH_Number          = TIMER_CHANNEL1;
   TIMER_ChnOutInit(MDR_TIMER1, &sTIM_ChnOutInit);
   /* Enable TIMER1 clock */
   TIMER_BRGInit(MDR_TIMER1,TIMER_HCLKdiv1);

	 //вкл прерываний
	 NVIC_EnableIRQ(TIMER1_IRQn);
	 TIMER_ITConfig(MDR_TIMER1, TIMER_STATUS_CNT_ZERO, ENABLE);
	 NVIC_SetPriority(TIMER1_IRQn, 0);
	 /* Enable TIMER1 */
   TIMER_Cmd(MDR_TIMER1,ENABLE);
}
//обработчик прерываний от 1-го таймера
void TIMER1_IRQHandler(void)
{
	//опрос всех каналов АЦП
	ADC_start_mesuare();
	
  TIMER_ClearITPendingBit(MDR_TIMER1, TIMER_STATUS_CNT_ZERO);
}


