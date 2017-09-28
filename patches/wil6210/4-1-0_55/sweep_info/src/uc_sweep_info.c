#pragma NEXMON targetregion "ucpatch"

#include <firmware_version.h>
#include <patcher.h>
#include <wrapper.h>
#include <helper.h>
#include "sweep_info.h"

#define PTR_CUR_SWEEP_SECTOR_INFO 0x801038
#define PTR_SWEEP_VALIDITY 0x80073D

// ---------------------------------------------------------------------------
// Global fixed variables
// ---------------------------------------------------------------------------

// The history of sweep packets in a dump
__attribute__((at(PTR_MEM_SWEEP_DUMP, "", CHIP_VER_WIL6210, FW_VER_410_55)))
sweep_dump_t sweep_dump = {0};

// Use printf for debugging
int printf(const char *fmt, ...);

int* const cur_sector_info = (int*) PTR_CUR_SWEEP_SECTOR_INFO;
uint32* const cur_pos = &(sweep_dump.cur_pos);
static uint16 last_id = 0;

typedef struct {
    uint32 rssi;
    int32 snr;
    uint32 sid;
    bool valid; 
} sector_info_buffer_t;
sector_info_buffer_t sector_info_buffer = {0};

void store_sweep_info_to_buffer(int passed_snr) {
    //sector_info_buffer.snr = cur_sector_info[2];
    sector_info_buffer.snr = (int16) passed_snr;
    sector_info_buffer.rssi = cur_sector_info[1];
    sector_info_buffer.sid = cur_sector_info[0];
    sector_info_buffer.valid = 1;  
}

void dump_sweep_info() {
    // At this point in time, the measurements taken for SNR and RSSI
    // are more up to date then the the sector ID and the frame buffer
    // To overcome this timing issue, we use a buffer to store the measurements

    sector_info_t* cur_dump;
    uint8* cur_frame_ssw_field = (uint8*) PTR_CUR_FRAME_BUFFER + 0x10;
    uint8* cur_src_addr = (uint8*) (PTR_CUR_FRAME_BUFFER + 0x0A);

    // Extract the current Frame Control Field from Frame Buffer
    uint16* cur_frame_control_field = (uint16*) PTR_CUR_FRAME_BUFFER;

    if (*cur_frame_control_field == 0x0864) {
	
    	// Received a sector sweep frame
        if (last_id > cur_sector_info[0]){
            // Increase the sweep counter
            sweep_dump.ctr_swps ++;
        }

	cur_dump = &sweep_dump.dump[(*cur_pos)];	
	cur_dump->sector_id = cur_frame_ssw_field[1] >> 2;

	if (sector_info_buffer.valid) {

	    // Add current information to dump
	    cur_dump->rssi = sector_info_buffer.rssi;
	    cur_dump->snr = sector_info_buffer.snr;
	} else {
	    // Measurement was incomplete
	    cur_dump->rssi = 0;
            cur_dump->snr = 0;
	}	

	// Copy the SRC MAC addr
	cur_dump->macaddr[0] = cur_src_addr[0];
	cur_dump->macaddr[1] = cur_src_addr[1];
	cur_dump->macaddr[2] = cur_src_addr[2];
	cur_dump->macaddr[3] = cur_src_addr[3];
	cur_dump->macaddr[4] = cur_src_addr[4];
	cur_dump->macaddr[5] = cur_src_addr[5];
	
	cur_dump->flags[0] = 0;
	cur_dump->flags[1] = 0;
	last_id = cur_sector_info[0];

	// Increase the counter
	sweep_dump.ctr_pkts ++;
	(*cur_pos) = ((*cur_pos) + 1) % SWEEP_DUMP_SIZE;

    // Mark the buffer invalid
    sector_info_buffer.valid = 0;
}

//Hook the processing sweep frame function
__attribute__((naked))
void
naked_store_sweep_frame_hook(void){
    asm (
        "push blink\n"
        "bl sub_A170\n"         // Call original function
        "mov r0, r5\n"
        "bl store_sweep_info_to_buffer\n"  // Dump the sweep info
        "pop blink\n"
        "j [blink]\n"
    );
}
__attribute__((at(UC_ADDR(0xD374), "", CHIP_VER_WIL6210, FW_VER_410_55)))
BLNEPatch(naked_store_sweep_frame_hook, naked_store_sweep_frame_hook);

__attribute__((at(UC_ADDR(0xD32E), "", CHIP_VER_WIL6210, FW_VER_410_55)))
BLPatch(dump_sweep_info, dump_sweep_info);

