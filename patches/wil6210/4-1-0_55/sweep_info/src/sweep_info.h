#ifndef SWEEP_INFO_H_
#define SWEEP_INFO_H_

#define SWEEP_DUMP_SIZE 256
#define FEEDBACK_OVERWRITE_SIZE 1

typedef struct {
    uint16_t sector_id;
    int16_t snr;
    uint32_t rssi;
    uint8_t macaddr[6];
    uint8_t flags[2];
} sector_info_t;

typedef struct {
    uint32_t cur_pos;
    uint32_t ctr_pkts;
    uint32_t ctr_swps;
    sector_info_t dump[SWEEP_DUMP_SIZE]; 
} sweep_dump_t;

#define PTR_MEM_SWEEP_CUR_FEEDBACK 0x93D000
#define PTR_MEM_SWEEP_DUMP 0x93D018

#define PTR_SEL_SWEEP_SECTOR_INFO 0x800F1C
#define PTR_CUR_SWEEP_SECTOR_INFO 0x801038
#define PTR_CUR_FRAME_BUFFER 0x804010

#endif /* SWEEP_INFO_H_ */
