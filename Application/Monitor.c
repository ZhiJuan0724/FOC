#include "Monitor.h"

char buff[128];
void UartPrintf(UART_HandleTypeDef *huart, char *fmt, ...)
{
  int ret;
  va_list ap;
  va_start(ap, fmt);
  ret = vsprintf(buff, fmt, ap);
  va_end(ap);
	
  if (ret > 0)
  {
    HAL_UART_Transmit(huart, (uint8_t *)buff, ret, HAL_MAX_DELAY);
  }
}
