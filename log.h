#ifndef __LOG_H__
#define __LOG_H__

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef unsigned long uint64_t;

// Constants
#define OP_ADD_NODE 0
#define OP_ADD_EDGE 1
#define OP_REMOVE_NODE 2
#define OP_REMOVE_EDGE 3


int format_disk();

int add_log(uint32_t opcode, uint64_t node_a_id, uint64_t node_b_id);

int checkpoint();

int recovery();


int print_all_log_block();

int print_all_checkpoint_block();

int new_next_block(int generation_num);

int restore();

int restore_check_point();

int restore_log();

int checksum_valid();

int update_checksum();
#endif
