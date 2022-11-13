#ifndef _MONITOR_H
#define _MONITOR_H

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "stm32f1xx_hal.h"

extern char buff[128];
void UartPrintf(UART_HandleTypeDef *huart, char *fmt, ...);

#endif
