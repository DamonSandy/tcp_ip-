/**
  ******************************************************************************
  * File Name          : TIM.c
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* USER CODE BEGIN 0 */
#include "usart.h"
#include "string.h"
#include "lwip_demo.h"
extern unsigned int system_tick_num;
extern char str[];
extern netif_device_one my_netif_device;
/* USER CODE END 0 */

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim7;

/* TIM1 init function */
void MX_TIM1_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 7199;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 9;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}
/* TIM7 init function */
void MX_TIM7_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig;

  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 7199;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 199;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM1)
  {
  /* USER CODE BEGIN TIM1_MspInit 0 */

  /* USER CODE END TIM1_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_TIM1_CLK_ENABLE();
  /* USER CODE BEGIN TIM1_MspInit 1 */

  /* USER CODE END TIM1_MspInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM7)
  {
  /* USER CODE BEGIN TIM7_MspInit 0 */

  /* USER CODE END TIM7_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_TIM7_CLK_ENABLE();
  /* USER CODE BEGIN TIM7_MspInit 1 */

  /* USER CODE END TIM7_MspInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM1)
  {
  /* USER CODE BEGIN TIM1_MspDeInit 0 */

  /* USER CODE END TIM1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM1_CLK_DISABLE();

    /* TIM1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM1_UP_IRQn);
  /* USER CODE BEGIN TIM1_MspDeInit 1 */

  /* USER CODE END TIM1_MspDeInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM7)
  {
  /* USER CODE BEGIN TIM7_MspDeInit 0 */

  /* USER CODE END TIM7_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM7_CLK_DISABLE();

    /* TIM7 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM7_IRQn);
  /* USER CODE BEGIN TIM7_MspDeInit 1 */

  /* USER CODE END TIM7_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
/***********************************更新中断回调函数*****************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)  
{
	if(htim->Instance == TIM1)
	{
		if(Is_rectimeover(&G_USART_deliver) == TRUE)
		{
//			HAL_UART_Transmit(&huart1,get_recbuf_adr(&G_USART_deliver),MAX_REC_NUM,1000);
			__HAL_TIM_DISABLE(htim);
			memcpy(str,G_USART_deliver.receive_buf,MAX_REC_NUM);
//			send_my_data_by_internet(&my_netif_device);
			clr_recbuf(&G_USART_deliver);
		}
	}
	if(htim->Instance == TIM7)
	{
		system_tick_num += 20;
	}
}
/*****************************延时相关代码********************************************/
void delay_init(void)
{
	 SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
										SysTick_CTRL_TICKINT_Pos   |
										SysTick_CTRL_ENABLE_Pos;       //使用内核时钟,关滴答定时器，不开中断
}
void delay_us(uint16_t us)
{
	uint16_t fa_us = HAL_RCC_GetHCLKFreq()/1000000U;
	SysTick->LOAD  = (uint32_t)(fa_us*us - 1UL); 
	SysTick->VAL   = 0UL;   
	SysTick->CTRL  |= SysTick_CTRL_ENABLE_Pos; //开滴答定时器
	while(!(SysTick->CTRL&(1<<16)));  //等待计数到零
	SysTick->CTRL &=~SysTick_CTRL_ENABLE_Pos;//关闭SysTick
}
void delay_ms(uint16_t ms)
{
	while(ms--)
	{
		delay_us(1000);
	}
}
/***********************************串口数据处理函数*****************************************/
void add_recbuf_value(t_USART_DATA *USART_deliver,uint8_t value)
{
	USART_deliver->receive_buf[USART_deliver->data_count] = value;
	USART_deliver->data_count++;
	USART_deliver->data_count %= MAX_REC_NUM;
	USART_deliver->rec_timecount = 0;
}
uint8_t Is_rectimeover(t_USART_DATA *USART_deliver)
{
	USART_deliver->rec_timecount++;
	if(USART_deliver->rec_timecount >= OVERTIME_NUM)
	{
		USART_deliver->rec_timecount = 0;
		USART_deliver->data_count = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}
void clr_recbuf(t_USART_DATA *USART_deliver)
{
	uint8_t i = 0;
	for(i=0; i<MAX_REC_NUM; i++)
	{
		USART_deliver->receive_buf[i] = 0;
	}
}
uint8_t *get_recbuf_adr(t_USART_DATA *USART_deliver)
{
	return USART_deliver->receive_buf;
}
uint8_t *get_recbf_adr(t_USART_DATA *USART_deliver)
{
	return USART_deliver->rec_bf;
}
/******************************************************************************/
/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
