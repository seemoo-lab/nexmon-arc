#pragma NEXMON targetregion "fwpatch"

#include <firmware_version.h>
#include <patcher.h>
#include <wrapper.h>
#include <helper.h>

void
fw_hook() {
    printf("FW: INITIALIZED\n");
    sub_28828();
}

__attribute__((at(FW_ADDR(0x180A), "", CHIP_VER_WIL6210, FW_VER_410_55)))
BLPatch(fw_hook, fw_hook);
