#pragma NEXMON targetregion "ucpatch"

#include <firmware_version.h>
#include <patcher.h>
#include <wrapper.h>
#include <helper.h>

void
uc_hook() {
    printf("UC: INITIALIZED\n");
    sub_1604();
}

__attribute__((at(UC_ADDR(0x1590), "", CHIP_VER_WIL6210, FW_VER_410_55)))
BLPatch(uc_hook, uc_hook);
