#include "stm32f1xx_hal.h"
#include <string.h>

#define LDO_EN_Pin GPIO_PIN_0
#define LDO_EN_GPIO_Port GPIOA

SPI_HandleTypeDef hspi1;
uint8_t spi_rx_buffer[4];

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
void process_spi_command(uint8_t* cmd);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_SPI1_Init();

  HAL_SPI_Receive_IT(&hspi1, spi_rx_buffer, sizeof(spi_rx_buffer));

  while (1)
  {
    // loop vazio, tudo via interrupção SPI
  }
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
  if (hspi->Instance == SPI1)
  {
    process_spi_command(spi_rx_buffer);
    HAL_SPI_Receive_IT(&hspi1, spi_rx_buffer, sizeof(spi_rx_buffer)); // reativa
  }
}

void process_spi_command(uint8_t* cmd)
{
  if (strncmp((char*)cmd, "on", 2) == 0)
  {
    HAL_GPIO_WritePin(LDO_EN_GPIO_Port, LDO_EN_Pin, GPIO_PIN_SET);
  }
  else if (strncmp((char*)cmd, "off", 3) == 0)
  {
    HAL_GPIO_WritePin(LDO_EN_GPIO_Port, LDO_EN_Pin, GPIO_PIN_RESET);
  }

  memset(spi_rx_buffer, 0, sizeof(spi_rx_buffer));
}

void MX_SPI1_Init(void)
{
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_SLAVE;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_HARD_INPUT;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;

  if (HAL_SPI_Init(&hspi1) != HAL_OK)
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
