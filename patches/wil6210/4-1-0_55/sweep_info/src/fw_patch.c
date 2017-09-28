#pragma NEXMON targetregion "fwpatch"

#include <firmware_version.h>
#include <patcher.h>
#include <wrapper.h>
#include <helper.h>

#define WMI_CMD_HDR_LEN 8

void
wmi_handler_hook(unsigned short wmi_id, unsigned short data_len, int *data) {
    data_len = data_len - WMI_CMD_HDR_LEN;

    printf("wmi_handler_hook() wmi_id: 0x%x, data len: %d\n", wmi_id, data_len);
}

__attribute__((naked))
void
naked_main(void) {
    asm (
        "push blink\n"
        "push r0\n"
        "push r1\n"
        "push r2\n"
        "push r3\n"
        "push r4\n"
        "push r5\n"
        "push r6\n"
        "push r7\n"
        "push r8\n"
        "push r9\n"
        "push r10\n"
        "push r11\n"
        "push r12\n"
        "push r13\n"
        "push r14\n"
        "push r15\n"
        "push r16\n"
        "push r17\n"
        "push r18\n"
        "mov r17, r1\n"
        //move wmi id to argument 1
        "ldw r3, [r17, 0xA]\n"
        "mov r0, r3\n"
        //data len
        "ldw r3, [r17, 0x2]\n"
        "mov r1, r3\n"
        //data start
        "add r2, r17, 0x10\n"

        "bl wmi_handler_hook\n"

        "pop r18\n"
        "pop r17\n"
        "pop r16\n"
        "pop r15\n"
        "pop r14\n"
        "pop r13\n"
        "pop r12\n"
        "pop r11\n"
        "pop r10\n"
        "pop r9\n"
        "pop r8\n"
        "pop r7\n"
        "pop r6\n"
        "pop r5\n"
        "pop r4\n"
        "pop r3\n"
        "pop r2\n"
        "pop r1\n"
        "pop r0\n"
        "pop blink\n"

        "b store_r13_to_r17\n"
        );
}

__attribute__((at(FW_ADDR(0x1A672), "", CHIP_VER_WIL6210, FW_VER_410_55)))
BLPatch(naked_main, naked_main);
