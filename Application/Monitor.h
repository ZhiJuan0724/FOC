#ifndef _MONITOR_H
#define _MONITOR_H
#include "stm32f1xx_hal.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

extern char buff[128];
void UartPrintf(UART_HandleTypeDef *huart, char *fmt, ...);

#endif
