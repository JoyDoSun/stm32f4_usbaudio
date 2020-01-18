#include "usb_bsp.h"  
 
#ifdef USE_USB_OTG_FS  
//OTG-FS GPIO��ʼ��:PA11,PA12
void USB_OTG_BSP_Init(USB_OTG_CORE_HANDLE *pdev)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
  RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS, ENABLE);//ʹ��USB OTGʱ��
  //GPIOA11,A12����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;//PA11/12���ù������	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_OTG_FS);//PA11,AF10(USB)
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_OTG_FS);//PA12,AF10(USB)
	
}
#endif


#ifdef USE_USB_OTG_HS  
//OTG-HS GPIO��ʼ��: PS14,PB15
void USB_OTG_BSP_Init(USB_OTG_CORE_HANDLE *pdev)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_OTG_HS, ENABLE);//ʹ��HS OTG ʱ��
  //PB14:OTG_HS_DM PB15:OTG_HS_DP
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;//PB14:OTG_HS_DM PB15:DP
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_OTG_HS_FS);//HS����FSģʽ
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_OTG_HS_FS);
	
}

#endif

/**
* @brief  USB_OTG_BSP_EnableInterrupt  �ж����
* @param  None
* @retval None
*/
void USB_OTG_BSP_EnableInterrupt(USB_OTG_CORE_HANDLE *pdev)
{
  NVIC_InitTypeDef NVIC_InitStructure; 
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
#ifdef USE_USB_OTG_HS   
  NVIC_InitStructure.NVIC_IRQChannel = OTG_HS_IRQn;
#else
  NVIC_InitStructure.NVIC_IRQChannel = OTG_FS_IRQn;  
#endif
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  
#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = OTG_HS_EP1_OUT_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = OTG_HS_EP1_IN_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);   
#endif
}

//USB_OTG us����ʱ����
void USB_OTG_BSP_uDelay (const uint32_t usec)
{
  uint32_t count = 0;
  const uint32_t utime = ((SystemCoreClock/1000000) * usec / 7);
  do
  {
    if ( ++count > utime )
    {
      return ;
    }
  }
  while (1);
}

//USB_OTG ms����ʱ����
void USB_OTG_BSP_mDelay (const uint32_t msec)
{
  USB_OTG_BSP_uDelay(msec * 1000);   
}

