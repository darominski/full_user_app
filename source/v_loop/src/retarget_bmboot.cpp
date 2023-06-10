#ifdef _DIOT

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

#include "bmboot_slave.hpp"

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

// **********************************************************

extern "C" void printfInit()
{
    // Disable I/O buffering for STDOUT stream
    setvbuf(stdout, NULL, _IONBF, 0);
}

// **********************************************************

extern "C" int _isatty(int fd)
{
    if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
        return 1;

    errno = EBADF;
    return 0;
}

// **********************************************************

extern "C" int _write(int fd, char *ptr, int len) {
    return bmboot_s::write_stdout(ptr, len);
}

// **********************************************************

extern "C" int _close(int fd)
{
    if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
        return 0;

    errno = EBADF;
    return -1;
}

// **********************************************************

extern "C" int _lseek(int fd, int ptr, int dir)
{
    (void) fd;
    (void) ptr;
    (void) dir;

    errno = EBADF;
    return -1;
}

// **********************************************************

extern "C" int _read(int fd, char* ptr, int len)
{
    if (fd == STDIN_FILENO)
    {
        return EIO;
    }
    errno = EBADF;
    return -1;
}

// **********************************************************

extern "C" int _fstat(int fd, struct stat* st)
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

extern "C" int _getpid(void)
{
    return 1;
}

// **********************************************************

extern "C" void _kill(int pid)
{
    while(1) ;
}
#endif
