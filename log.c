#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include "log.h"
#include "graph.h"

#ifndef O_DIRECT
#define O_DIRECT 0
#endif


int BLOCK_SIZE = 4096;
int MAXIMUM_ENTRY_NUM = 200;
int START_LOG_SEG = 1;
int START_CHECKPOINT_SEG = 0.5 * 1024 * 1024;
int CHECKSUM_POSITION_IN_LOG_BLOCK = 4092;

int DEBUG_LOG = 1;
extern int64_t delimiter_for_node;
extern int64_t delimiter_for_graph;
extern Graph *graph;
extern char* log_filename;


int add_log(uint32_t opcode, uint64_t node_a_id, uint64_t node_b_id) {
	/*
	1. Get the right position for the new entry
	2. Write Entry into block
	3. Update entry_num
	4. Update checksum
	*/
	if(DEBUG_LOG) {
		printf("Start the process of adding log...\n");
	}
	int fd;
	if(( fd = open(log_filename, O_RDWR | O_DIRECT) ) < 0) {
		perror("open");
		return -1;
	}

	// Get log area last block num:
	lseek(fd, 12, SEEK_SET);
	int tail;
	read(fd, &tail, 4);

	int last_block_position = BLOCK_SIZE * tail;
	
	// Get generation_num & entry_num from previous block:
	lseek(fd, last_block_position, SEEK_SET);
	int generation_num;
	int entry_num;
	read(fd, &generation_num, 4);
	read(fd, &entry_num, 4);
	
	// Add log entry:
	if(entry_num > MAXIMUM_ENTRY_NUM - 1) { // If current block is full, construct a new block
		new_next_block(generation_num);
	}

	last_block_position = BLOCK_SIZE * tail;
	int entry_start_position_in_block = 8;
	lseek(fd, last_block_position + entry_start_position_in_block + entry_num * 20, SEEK_SET);
	write(fd, &opcode, 4);
	write(fd, &node_a_id, 8);
	write(fd, &node_b_id, 8);

	// Entry_num ++ and write back
	entry_num++;
	lseek(fd, last_block_position + 4, SEEK_SET);
	write(fd, &entry_num, 4);
	
	// Update checksum for superblock
	// Get log area last block num:
	int start_log_seg, size_log_seg, head;
	lseek(fd, 0, SEEK_SET);
	read(fd, &start_log_seg, 4);
	read(fd, &size_log_seg, 4);
	read(fd, &head, 4);
	read(fd, &tail, 4);
	read(fd, &generation_num, 4);

	int checksum = start_log_seg ^ size_log_seg ^ head ^ tail ^ generation_num;

	write(fd, &checksum, 4);
	// Update checksum for current block

	close(fd);

	if(DEBUG_LOG) {
		print_all_log_block();
		printf("Adding log finished!\n");
	}
	return 0;
}





int checkpoint() {
	/*
	1. Store current graph to disk
	2. Add a new block to the log area
	*/
	if(DEBUG_LOG) {
		printf("Start the process of storing the graph...\n");
	}
	int fd;
  	if((fd = open(log_filename, O_RDWR | O_DIRECT )) < 0){
    	perror("open:");
    	return -1;
  	}

  	// Store current graph to disk
  	uint64_t pos = START_CHECKPOINT_SEG;
  	pos *= BLOCK_SIZE;    // must write like this! If write START_CHECKPOINT_SEG * BLOCK_SIZE will cause overflow
  	lseek(fd, pos, SEEK_SET);

  	AdjList *curr = graph->adjList;
	while(curr != NULL) {
		write(fd, &curr->node_id, 8);
		AdjListNode *tmp = curr->head;
		while(tmp != NULL) {
			write(fd, &tmp->node_id, 8);
			tmp = tmp->next;
		}
		curr = curr->next;
		write(fd, &delimiter_for_node, 8);
	}
	write(fd, &delimiter_for_graph, 8);
	
	// Add a new block
	// First find generation number to use next
	lseek(fd, 16, SEEK_SET);
	int generation_num;
	read(fd, &generation_num, 4);
	generation_num++;
	new_next_block(generation_num);
	// Then Update head to be same with tail
	lseek(fd, 12, SEEK_SET);
	int tail;
	read(fd, &tail, 4);
	lseek(fd, 8, SEEK_SET);
	write(fd, &tail, 4);

	if(DEBUG_LOG) {
		print_all_checkpoint_block();
		print_all_log_block();

	}
	close(fd);
	if(DEBUG_LOG) {
		printf("Storing the graph finished!\n");
	}
	return 0;
}

int restore() {
	/*
	1. Restore from check point
	2. Restore from log
	*/
	if(DEBUG_LOG) {
		printf("######## Start to restore ######## \n");
	}

	if(checksum_valid() == -1) {
		return -1;
	} 
	if(restore_check_point() == -1 || restore_log() == -1) {
		return -1;
	}
	if(DEBUG_LOG) {
		printf("######## Restoring finished ######## \n");
	}
	return 0;
}

int checksum_valid() {
	/*
	check whether the superblock is valid or not
	*/
	if(DEBUG_LOG) {
		printf("######## Superblock validation start! ######## \n");
	}
	int fd;
  	if((fd = open(log_filename, O_RDWR | O_DIRECT )) < 0){
    	perror("open:");
    	return -1;
  	}
	int start_log_seg, size_log_seg, head, tail, generation_num, checksum;
	lseek(fd, 0, SEEK_SET);
	read(fd, &start_log_seg, 4);
	read(fd, &size_log_seg, 4);
	read(fd, &head, 4);
	read(fd, &tail, 4);
	read(fd, &generation_num, 4);
	read(fd, &checksum, 4);
	close(fd);
	if(checksum != (start_log_seg ^ size_log_seg ^ head ^ tail ^ generation_num)) {
		if(DEBUG_LOG) {
			printf("######## Superblock is not valid! ######## \n");
			print_all_log_block();
		}

		return -1;
	}
	if(DEBUG_LOG) {
		printf("######## Superblock is valid! ######## \n");
	}

	return 0;
}
int restore_check_point() {
	/*
	1. Initialize the graph
	2. Restore from checkpoint
	*/
	if(DEBUG_LOG) {
		printf("######## Start to restore from checkpoint: ######## \n");
	}
	int fd;
	if(( fd = open(log_filename, O_RDONLY | O_DIRECT) ) < 0) {
		perror("open");
    	return -1;
	}

	// Initialize the graph
	graph_init();

	// Restore from checkpoint
	/* Checkpoint format:
	0 0 
	1 0 2 3 4 
	2 0 1 3 
	3 0 1 2 
	4 0 1 5 
	5 0 4 
	*/

	int64_t tmp;
	uint64_t pos = START_CHECKPOINT_SEG;
	pos *= BLOCK_SIZE;
	lseek(fd, pos, SEEK_SET);
	
	read(fd, &tmp, 8);
	int64_t curr_node_id = -1; // curr_node_id = -1 means that the new head of list is not avilable
	while(tmp != delimiter_for_graph) {
		if(tmp == 0) {
			read(fd, &tmp, 8);
			continue;
		}
		if(tmp == delimiter_for_node)  {
			curr_node_id = -1;
			read(fd, &tmp, 8);
			continue;
		}
		else {
			if(curr_node_id == -1) {
				curr_node_id = tmp; // set current node as the head of the list
				add_node(curr_node_id);
			}
			else {
				add_node(tmp);
				add_edge(curr_node_id, tmp);
			}
			read(fd, &tmp, 8);
		}
	}
	close(fd);

	if(DEBUG_LOG) {
		printf("######## Restore from checkpoint finished ######## \n");
		printAdjList();
	}
	return 0;
}

int restore_log() {
	/*
	1. Find the position of the log: From head to tail
	2. Restore from the log
	*/
	if(DEBUG_LOG) {
		printf("######## Start to restore from log: ######## \n");
	}

	int fd;
	if(( fd = open(log_filename, O_RDONLY | O_DIRECT) ) < 0) {
		perror("open");
    	return -1;
	}
	
	int head, tail;
	lseek(fd, 8, SEEK_SET);
	read(fd, &head, 4);
	read(fd, &tail, 4);

	
	for(int64_t i = head; i <= tail; i++) {
		int64_t block_position = BLOCK_SIZE;
		block_position *= i; 
		lseek(fd, block_position + 4, SEEK_SET);
		int entry_num;
		read(fd, &entry_num, 4);
		for(int j = 0; j < entry_num; j++) {
			int opcode;
			int64_t node_a_id, node_b_id;
			read(fd, &opcode, 4);
			read(fd, &node_a_id, 8);
			read(fd, &node_b_id, 8);
			//0 for add_node, 1 for add_edge, 2 for remove_node, 3 for remove_edge
			switch(opcode) {
				case 0: add_node(node_a_id); break;
				case 1: add_edge(node_a_id, node_b_id); break;
				case 2: remove_node(node_a_id); break;
				case 3: remove_edge(node_a_id, node_b_id); break;
				default: printf("Unknown opcode!\n"); break;
			}
		}
	}

	close(fd);
	if(DEBUG_LOG) {
		printAdjList();
		printf("######## Restore from log finished ######## \n");
	}
	return 0;

}


int format_disk() {
	/*
	1. Construct the super block
	2. Construct the first log block
	*/
	if(DEBUG_LOG) {
		printf("Start to format disk...\n");
	}

	int fd;
	if(( fd = open(log_filename, O_RDWR | O_TRUNC | O_DIRECT) ) < 0) {
		perror("open");
    	return -1;
	}

	// Construct the super block
	
	unsigned int start_log_seg = START_LOG_SEG; // start of log segment (e.g. block 1)
	unsigned int size_log_seg = START_CHECKPOINT_SEG;// the log size to be the first 2GB: 2GB / 4 KB
	unsigned int head = 1; // current head of the log (e.g. block 55)
	unsigned int tail = 0; // current tail of the log
	unsigned int generation_num = 0;

	lseek(fd, 0, SEEK_SET);
	write(fd, &start_log_seg , 4);
	write(fd, &size_log_seg	 , 4);
	write(fd, &head			 , 4);
	write(fd, &tail			 , 4);
	write(fd, &generation_num, 4);

	unsigned int checksum = 0;
	checksum = start_log_seg ^ size_log_seg ^ head ^ tail ^ generation_num;
	write(fd, &checksum, 4);

	new_next_block(generation_num);
	close(fd);
	
	if(DEBUG_LOG) {
		print_all_log_block();
		printf("Formatting disk finished\n");
	}
	
	return 0;
}

int new_next_block(int generation_num) {
	/*
	Construct a new block with given generation_num:
	1. Get the current tail
	2. Update the tail 
	3. Construct a new block at tail with given generation_num (and entry_num is 0)
	4. Update the checksum
	*/
	if(DEBUG_LOG) {
		printf("**** Start to construct a new block for generation %d: **** \n", generation_num);
	}
	int fd;
	if(( fd = open(log_filename, O_RDWR | O_DIRECT ) ) < 0) {
		perror("open");
		return -1;
	}

	// Get log area last block num
	lseek(fd, 12, SEEK_SET);
	int tail;
	read(fd, &tail, 4);
	tail++;
	lseek(fd, 12, SEEK_SET);
	write(fd, &tail, 4);
	
	int new_block_position = BLOCK_SIZE * tail;

	// Write generation_num & entry_num for new block
	lseek(fd, new_block_position, SEEK_SET);
	int entry_num = 0;
	write(fd, &generation_num, 4);
	write(fd, &entry_num, 4);


	if(DEBUG_LOG) {
		printf("**** Constructing a new block finished!\n");
	}
	return 0;
}

int print_all_checkpoint_block() {
	if(DEBUG_LOG) {
		printf("######## Checkpoint block status: ######## \n");
	}

	int fd;
	if(( fd = open(log_filename, O_RDONLY | O_DIRECT ) ) < 0) {
		perror("open");
    	return -1;
	}

	int64_t tmp;
	uint64_t pos = START_CHECKPOINT_SEG;
	pos *= BLOCK_SIZE;
	lseek(fd, pos, SEEK_SET);
	read(fd, &tmp, 8);
	while(tmp != delimiter_for_graph) {
		if(tmp == delimiter_for_node)  {
			printf("\n");
		}
		else {
			printf("%llu ", tmp);
		}
		read(fd, &tmp, 8);
	}
	close(fd);
	if(DEBUG_LOG) {
		printf("######## Printing Checkpoint Finished ######## \n");
	}
	return 0;
}

int print_all_log_block() {
	/*
	Print all log block (contains unuseful logs)
	*/
	int fd;
	if(( fd = open(log_filename, O_RDONLY | O_DIRECT) ) < 0) {
		perror("open");
    	return -1;
	}

	if(DEBUG_LOG) {
		printf("######## Current block status: ######## \n");
	}

	int start_log_seg, size_log_seg, head, tail, generation_num, checksum;

	lseek(fd, 0, SEEK_SET);
	read(fd, &start_log_seg, 4);
	read(fd, &size_log_seg, 4);
	read(fd, &head, 4);
	read(fd, &tail, 4);
	read(fd, &generation_num, 4);
	read(fd, &checksum, 4);

	printf("**** Super block: ****\n");
	printf("start_log_seg %d\n", start_log_seg);
	printf("size_log_seg %d\n", size_log_seg);
	printf("head %d\n", head);
	printf("tail %d\n", tail);
	printf("generation_num %d\n", generation_num);
	printf("checksum %d\n", checksum);
	
	printf("**** Normal log block: ****\n");
	for(int64_t i = 1; i <= tail; i++) {
		printf("$$ Block %lld: $$\n", i);
		int64_t block_position = BLOCK_SIZE;
		block_position *= i; 
		int entry_num;
		lseek(fd, block_position, SEEK_SET);
		read(fd, &generation_num, 4);
		read(fd, &entry_num, 4);
		printf("generation_num: %d\n", generation_num);
		printf("entry_num: %d\n", entry_num);
		for(int j = 0; j < entry_num; j++) {
			int opcode;
			int64_t node_a_id, node_b_id;
			read(fd, &opcode, 4);
			read(fd, &node_a_id, 8);
			read(fd, &node_b_id, 8);
			//0 for add_node, 1 for add_edge, 2 for remove_node, 3 for remove_edge
			printf("%d %llu %lld \n", opcode, node_a_id, node_b_id);
		}
	}

	if(DEBUG_LOG) {
		printf("######## Current block status finished ######## \n");
	}
	return 0;
}
