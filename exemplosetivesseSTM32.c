#include "stm32f1xx_hal.h"

#define LDO_EN_Pin GPIO_PIN_0
#define LDO_EN_GPIO_Port GPIOA

#define BTN_Pin GPIO_PIN_1
#define BTN_GPIO_Port GPIOA

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

  int button_state = 0;
  int ldo_on = 0;

  while (1)
  {
    if (HAL_GPIO_ReadPin(BTN_GPIO_Port, BTN_Pin) == GPIO_PIN_SET && button_state == 0)
    {
      button_state = 1;
      ldo_on = !ldo_on;

      HAL_GPIO_WritePin(LDO_EN_GPIO_Port, LDO_EN_Pin, ldo_on ? GPIO_PIN_SET : GPIO_PIN_RESET);
      HAL_Delay(300); // debounce
    }

    if (HAL_GPIO_ReadPin(BTN_GPIO_Port, BTN_Pin) == GPIO_PIN_RESET)
    {
      button_state = 0;
    }
  }
}

void MX_GPIO_Init(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  // LDO_EN como saída
  GPIO_InitStruct.Pin = LDO_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LDO_EN_GPIO_Port, &GPIO_InitStruct);

  // Botão como entrada
  GPIO_InitStruct.Pin = BTN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BTN_GPIO_Port, &GPIO_InitStruct);
}
