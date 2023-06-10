#include <_ansi.h>
#include <_syslist.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/times.h>
#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>

#include "retarget.h"

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

// **********************************************************

void printfInit()
{
    // Disable I/O buffering for STDOUT stream
    setvbuf(stdout, NULL, _IONBF, 0);
}

// **********************************************************

int _isatty(int fd)
{
    if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
        return 1;

    errno = EBADF;
    return 0;
}

// **********************************************************

#ifdef _DIOT

#include "xuartps.h"

int _write(int fd, char *ptr, int len) {
    int SentCount = 0;
    int Status;
    XUartPs_Config *Config;

    static int configured = 0;
    static XUartPs Uart_Ps;

    if (!configured) {
        /*
         * Initialize the UART driver so that it's ready to use
         * Look up the configuration in the config table and then initialize it.
         */
        Config = XUartPs_LookupConfig(XPAR_XUARTPS_0_DEVICE_ID);
        if (NULL == Config) {
            return XST_FAILURE;
        }

        Status = XUartPs_CfgInitialize(&Uart_Ps, Config, Config->BaseAddress);
        if (Status != XST_SUCCESS) {
            return XST_FAILURE;
        }

        XUartPs_SetBaudRate(&Uart_Ps, 115200);
        configured = 1;
    }

    // TODO: why 1-by-1 ?
    while (SentCount < len) {
        SentCount += XUartPs_Send(&Uart_Ps, (uint8_t*) &ptr[SentCount], 1);
    }

    return SentCount;
}

#endif

// **********************************************************

int _close(int fd)
{
    if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
        return 0;

    errno = EBADF;
    return -1;
}

// **********************************************************

int _lseek(int fd, int ptr, int dir)
{
    (void) fd;
    (void) ptr;
    (void) dir;

    errno = EBADF;
    return -1;
}

// **********************************************************

int _read(int fd, char* ptr, int len)
{
    if (fd == STDIN_FILENO)
    {
        /*hstatus = HAL_UART_Receive(gHuart, (uint8_t *) ptr, 1, HAL_MAX_DELAY);
        if (hstatus == HAL_OK)
            return 1;
        else*/
        return EIO;
    }
    errno = EBADF;
    return -1;
}

// **********************************************************

int _fstat(int fd, struct stat* st)
{
    if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
    {
        st->st_mode = S_IFCHR;
        return 0;
    }

    errno = EBADF;
    return 0;
}

// **********************************************************

int _getpid(void)
{
    return 1;
}

// **********************************************************

void _kill(int pid)
{
    while(1) ;
}
