#pragma NEXMON targetregion "fwpatch"
#pragma NEXMON targetregion "ucpatch"

#include <firmware_version.h>

#define LOG_BUF_LEN     (2*1024)
#define NULL            0

typedef unsigned int uint;
typedef unsigned int size_t;

typedef struct {
    char        *buf;
    uint        buf_size;
    uint        idx;
} hndrte_log_t;

static char console_buf[LOG_BUF_LEN] = {0};

// uc console
__attribute__((at(0x935ff4, "", CHIP_VER_WIL6210, FW_VER_7759)))
// fw console
__attribute__((at(0x8ffff0, "", CHIP_VER_WIL6210, FW_VER_7759)))
// uc console
__attribute__((at(0x935ff4, "", CHIP_VER_WIL6210, FW_VER_7780)))
// fw console
__attribute__((at(0x8ffff0, "", CHIP_VER_WIL6210, FW_VER_7780)))
// uc console
__attribute__((at(0x9386f4, "", CHIP_VER_WIL6210, FW_VER_410_55)))
// fw console
__attribute__((at(0x8ffff0, "", CHIP_VER_WIL6210, FW_VER_410_55)))
static hndrte_log_t active_log = {
    .buf = console_buf,
    .buf_size = LOG_BUF_LEN,
    .idx = 0
};

void
clearconsole(void)
{
    int i = 0;
    for (i = 0; i < LOG_BUF_LEN; i++) {
        console_buf[i] = 0;
    }

    active_log.idx = 0;
}

void
putc(int c)
{
    hndrte_log_t *log = &active_log;

    /* CR before LF */
    if (c == '\n')
        putc('\r');

    if (log->buf != NULL) {
        int idx = log->idx;

        /* Store in log buffer */
        log->buf[idx] = (char)c;
        log->idx = (idx + 1) % LOG_BUF_LEN;
    }
}
