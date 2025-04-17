#include "stm32f1xx_hal.h"
#include <string.h>

#define LDO_EN_Pin GPIO_PIN_0
#define LDO_EN_GPIO_Port GPIOA

UART_HandleTypeDef huart1;
uint8_t rx_buffer[10];

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
void process_uart_command(uint8_t* cmd);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART1_UART_Init();

  HAL_UART_Receive_IT(&huart1, rx_buffer, sizeof(rx_buffer));

  while (1)
  {
    // Loop principal vazio, tudo feito via interrupção
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    process_uart_command(rx_buffer);
    HAL_UART_Receive_IT(&huart1, rx_buffer, sizeof(rx_buffer));
  }
}

void process_uart_command(uint8_t* cmd)
{
  if (strncmp((char*)cmd, "on", 2) == 0)
  {
    HAL_GPIO_WritePin(LDO_EN_GPIO_Port, LDO_EN_Pin, GPIO_PIN_SET);
    HAL_UART_Transmit(&huart1, (uint8_t*)"LDO ON\r\n", 8, HAL_MAX_DELAY);
  }
  else if (strncmp((char*)cmd, "off", 3) == 0)
  {
    HAL_GPIO_WritePin(LDO_EN_GPIO_Port, LDO_EN_Pin, GPIO_PIN_RESET);
    HAL_UART_Transmit(&huart1, (uint8_t*)"LDO OFF\r\n", 9, HAL_MAX_DELAY);
  }
  else
  {
    HAL_UART_Transmit(&huart1, (uint8_t*)"Comando invalido\r\n", 18, HAL_MAX_DELAY);
  }

  memset(rx_buffer, 0, sizeof(rx_buffer));
}

void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;

  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
}

void MX_GPIO_Init(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Pin = LDO_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LDO_EN_GPIO_Port, &GPIO_InitStruct);
}
