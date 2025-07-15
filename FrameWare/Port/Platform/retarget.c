#include <errno.h>
#include <sys/time.h>
#include <Platform/retarget.h>
#include "usart.h"

#if !defined(OS_USE_SEMIHOSTING)

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

UART_HandleTypeDef *gHuart;


/*重新定义 _write函数*/
int _write(int fd, char *ptr, int len)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, 0xFF);
    return len;
}


#endif //#if !defined(OS_USE_SEMIHOSTING)