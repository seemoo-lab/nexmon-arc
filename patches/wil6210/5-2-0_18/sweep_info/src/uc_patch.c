/***************************************************************************
 *                                                                         *
 *          ###########   ###########   ##########    ##########           *
 *         ############  ############  ############  ############          *
 *         ##            ##            ##   ##   ##  ##        ##          *
 *         ##            ##            ##   ##   ##  ##        ##          *
 *         ###########   ####  ######  ##   ##   ##  ##    ######          *
 *          ###########  ####  #       ##   ##   ##  ##    #    #          *
 *                   ##  ##    ######  ##   ##   ##  ##    #    #          *
 *                   ##  ##    #       ##   ##   ##  ##    #    #          *
 *         ############  ##### ######  ##   ##   ##  ##### ######          *
 *         ###########    ###########  ##   ##   ##   ##########           *
 *                                                                         *
 *            S E C U R E   M O B I L E   N E T W O R K I N G              *
 *                                                                         *
 * This file is part of NexMon.                                            *
 *                                                                         *
 * Copyright (c) 2018 NexMon Team                                          *
 *                                                                         *
 * NexMon is free software: you can redistribute it and/or modify          *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 * NexMon is distributed in the hope that it will be useful,               *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License       *
 * along with NexMon. If not, see <http://www.gnu.org/licenses/>.          *
 *                                                                         *
 **************************************************************************/

#pragma NEXMON targetregion "ucpatch"

#include <firmware_version.h>
#include <patcher.h>
#include <wrapper.h>
#include <helper.h>

#define PTR_CUR_FRAME_BUFFER 0x804010
#define SWEEP_DUMP_SIZE 256

typedef struct {
	uint8_t src[6];
	uint8_t swp[3];
	int16_t snr;
	uint16_t ctr;
	uint16_t flags;
} sector_info_t;

typedef struct {
    uint32_t cur_pos;
    uint32_t ctr_pkts;
    uint32_t ctr_swps;
    sector_info_t dump[SWEEP_DUMP_SIZE]; 
} sweep_dump_t;

int16_t snr_buffer = 0;
uint8_t snr_buffer_val = 0;
uint8_t lst_cdown = 0;

#define PTR_MEM_SWEEP_DUMP 0x93D018

// The history of sweep packets in a dump
__attribute__((at(PTR_MEM_SWEEP_DUMP, "", CHIP_VER_WIL6210, FW_VER_520_18)))
sweep_dump_t sweep_dump = {0};
uint32* const cur_pos = &(sweep_dump.cur_pos);

/*
 * Hook the initialization function and print debug output
 */
void
uc_hook() {
    printf("UC: INITIALIZED\n");
    sub_8bc();
}
__attribute__((at(UC_ADDR(0x82c), "", CHIP_VER_WIL6210, FW_VER_520_18)))
BLPatch(uc_hook, uc_hook);


/*
 * Hook the incoming sweep frames
 */
void
rx_sweep_frame() {
        sector_info_t* cur_dump;
	uint16* frame_control_field = (uint16*) PTR_CUR_FRAME_BUFFER;
        uint8* frame_src_addr = (uint8*) (PTR_CUR_FRAME_BUFFER + 0x0A);
	uint8* frame_ssw_field = (uint8*) PTR_CUR_FRAME_BUFFER + 0x10;
	uint8 cur_cdown;

	if (*frame_control_field == 0x0864) {
		// Received Sector Sweep Frame
		if (1) {
			// Add current sweep to dump
			cur_dump = &sweep_dump.dump[(*cur_pos)];
			
			// Check the CDOWN value
			cur_cdown = (frame_ssw_field[0] >> 1) + ((frame_ssw_field[1] & 0x03) << 7);
			if (cur_cdown >= lst_cdown){
				sweep_dump.ctr_swps ++;
			}
			lst_cdown = cur_cdown;

			// Use SNR value if valid
			if(snr_buffer_val ==1) {
            			cur_dump->snr = snr_buffer;
			} else {
				cur_dump->snr = 0x8000;
			}

			// Copy the SRC MAC addr
			cur_dump->src[0] = frame_src_addr[0];
			cur_dump->src[1] = frame_src_addr[1];
			cur_dump->src[2] = frame_src_addr[2];
			cur_dump->src[3] = frame_src_addr[3];
			cur_dump->src[4] = frame_src_addr[4];
			cur_dump->src[5] = frame_src_addr[5];

			cur_dump->swp[0] = frame_ssw_field[0];
			cur_dump->swp[1] = frame_ssw_field[1];
			cur_dump->swp[2] = frame_ssw_field[2];

			cur_dump->flags = 0;
			cur_dump->ctr = sweep_dump.ctr_swps;
			
			// Increase the counter
			sweep_dump.ctr_pkts ++;
			(*cur_pos) = ((*cur_pos) + 1) % SWEEP_DUMP_SIZE;	
		}
	}
	else if (*frame_control_field == 0x0a64 || *frame_control_field == 0x0964) {
		// Received Acknowledgement or Feedback
		printf("SWP Feedback (type %04x) from %02x:%02x:%02x:%02x:%02x:%02x sec: %d, snr: %02d dB\n",
                	*frame_control_field, frame_src_addr[0], frame_src_addr[1],
                	frame_src_addr[2], frame_src_addr[3], frame_src_addr[4],
                	frame_src_addr[5], frame_ssw_field[0] & 0x3F, 
			frame_ssw_field[1]/4+19);
	}
	else if (*frame_control_field == 0x0464 || *frame_control_field == 0x0764) {
                // Received Grant
                printf("SWP Grant (type %04x) from %02x:%02x:%02x:%02x:%02x:%02x\n",
                        *frame_control_field, frame_src_addr[0], frame_src_addr[1],
                        frame_src_addr[2], frame_src_addr[3], frame_src_addr[4],
                        frame_src_addr[5]);
        }
}
__attribute__((at(UC_ADDR(0xF486), "", CHIP_VER_WIL6210, FW_VER_520_18)))
BLPatch(rx_sweep_frame, rx_sweep_frame);

int16_t
hook_cache(){
	snr_buffer = sub_7e60();
	snr_buffer_val = 1;
	return snr_buffer;
}
__attribute__((at(UC_ADDR(0xF4D4), "", CHIP_VER_WIL6210, FW_VER_520_18)))
BLDPatch(hook_cache, hook_cache, "stw r3, [gp,0x66]\n");
