#include <stdio.h>
#include <stdlib.h>
#include "graph.h"


Graph *graph;
const int DEBUG = 1;
void graph_init() {
	graph = (struct Graph*) malloc(sizeof(struct Graph));
	graph->V = 0;
	
	graph->adjList = (struct AdjList*) malloc(sizeof(struct AdjList));
	graph->adjList->next = NULL;
	graph->adjList->node_id = 0;
	graph->adjList->key = 0;

	graph->adjList->head = (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
	graph->adjList->head->node_id = 0;
	graph->adjList->head->next = NULL;
	graph->adjList->head->key = 0;
}


int add_node(uint64_t node_id) {
	if(DEBUG) {
		printf("Start the process of adding node %llu...\n", node_id);
	}
	// check the node already exists or not
	// If already exists, return -1
	// Else prev will be set as the tail of the node
	// printf("*Breakpoint0\n");
	AdjList *curr = graph->adjList;
	AdjList *prev = graph->adjList;
	if(curr == NULL) {
		printf("curr == NULL!\n");
		return -1;
	}
	//skip the dummy node
	curr = curr->next;
	while(curr != NULL) {
		if(curr->node_id == node_id) {
			if(DEBUG) {
				printf("Node %llu alreay exists!\n", node_id);
			}
			return -1;
		}
		prev = curr;
		curr = curr->next;
	}

	// add node
	if(DEBUG) {
		printf("Now adding node %llu...\n", node_id);
	}
	prev->next = (AdjList *)malloc(sizeof(AdjList));
	prev = prev->next;
	
	graph->V += 1;
	prev->key = graph->V;
	prev->head = (AdjListNode *)malloc(sizeof(AdjListNode));
	prev->head->node_id = 0;
	prev->head->next = NULL;
	prev->head->key = 0;

	prev->next = NULL;
	prev->node_id = node_id;
	if(DEBUG) {
		printf("Adding node %llu completed!\n", node_id);
		printAdjList();
	}
	return 0;
}

int get_node(uint64_t node_id) {
	if(DEBUG) {
		printf("Start the process of getting node %llu...\n", node_id);
	}
	// printf("*Breakpoint0\n");
	AdjList *curr = graph->adjList;
	if(curr == NULL) {
		printf("curr == NULL!\n");
		return -1;
	}
	curr = curr->next;
	while(curr != NULL) {
		if(curr->node_id == node_id) {
			if(DEBUG) {
				printf("Node %llu exists!\n", node_id);
				printAdjList();
			}
			return 0;
		}
		curr = curr->next;
	}
	if(DEBUG) {
		printf("Node %llu does not exist!\n", node_id);
		printAdjList();
	}
	return -1;
}

int remove_node(uint64_t node_id) {
	if(DEBUG) { 
		printf("Start the process of removing node %llu...\n", node_id);
	}
	AdjList *curr = graph->adjList;
	AdjList *prev = curr;
	if(curr == NULL) {
		printf("curr == NULL!\n");
		return -1;
	}
	curr = curr->next;
	while(curr != NULL) {
		if(curr->node_id == node_id) {
			break;
		}
		// printf("*Breakpoint1\n");
		prev = curr;
		// printf("*Breakpoint2\n");
		curr = curr->next;
	}
	if(curr == NULL) {
		if(DEBUG) {
			printf("Node %llu does not exist!\n", node_id);
			printAdjList();
		}
		return -1;
	}
	else {
		if(DEBUG) {
			printf("Now deleting node %llu...\n", node_id);
		}
		prev->next = curr->next;
		if(DEBUG) {
			printf("Deleting all the edges that connected to node %llu...\n", node_id);
			curr = graph->adjList;
			if(curr == NULL) {
				printf("curr == NULL!\n");
				return -1;
			}
			curr = curr->next;
			while(curr != NULL) {
				AdjListNode *currAdjListNode = curr->head;
				AdjListNode *prevAdjListNode = currAdjListNode;
				if(currAdjListNode == NULL) {
					printf("curr == NULL!\n");
					return -1;
				}
				currAdjListNode = currAdjListNode -> next;
				while(currAdjListNode != NULL) {
					if(currAdjListNode -> node_id == node_id) {
						break;
					}
					prevAdjListNode = currAdjListNode;
					currAdjListNode = currAdjListNode -> next;
				}
				if(currAdjListNode != NULL) {
					prevAdjListNode->next = currAdjListNode->next;
				}
				curr = curr -> next;
			}
		}
		// graph->V -= 1;
		if(DEBUG) {
			printf("Deleting node %llu completed!\n", node_id);
			printAdjList();
		}
		return 0;
	}

}

int add_edge(uint64_t node_a_id, uint64_t node_b_id){
	if(DEBUG) {
		printf("Start the process of adding edge between %llu, %llu...\n", node_a_id, node_b_id);
	}
	/* return value:
		1. node_a_id == node_b_id : -1
		2. a or b does not exist: -2
		3. edge between a and b already exists: -3
		4. success: 0
	*/
	if(node_a_id == node_b_id) {
		if(DEBUG) {
			printf("Cannot add edge to the single node %llu!\n", node_a_id);
		}
		return -1;
	}
	AdjList *currAdjList = graph->adjList;
	AdjList *headOfA = NULL;
	AdjList *headOfB = NULL;
	if(currAdjList == NULL) {
		printf("curr == NULL!\n");
		return -1;
	}
	currAdjList = currAdjList->next;
	while(currAdjList != NULL) {
		if(currAdjList->node_id == node_a_id) {
			headOfA = currAdjList;
		}
		if(currAdjList->node_id == node_b_id) {
			headOfB = currAdjList;
		}
		if(headOfA != NULL && headOfB != NULL) {
			break;
		}
		// printf("*Breakpoint1\n");
		// printf("*Breakpoint2\n");
		currAdjList = currAdjList->next;
	}
	if(headOfA == NULL || headOfB == NULL) {
		if(DEBUG) { 
			printf("NodeA or(and) NodeB does not exists!\n");
			printAdjList();
		}
		return -2;
	}
	// Add edge to A:
	if(DEBUG) {
		printf("Start to add B to A's adjandency list...\n");
	}
	AdjListNode *currAdjListNode = headOfA->head;
	AdjListNode *prevAdjListNode = currAdjListNode;
	// printf("Breakpoint0\n");
	if(currAdjListNode == NULL) {
		if(DEBUG) {
			printf("currAdjListNode == NULL. Something wired happens...\n");
		}
		return -3;
	}
	currAdjListNode = currAdjListNode->next;
	while(currAdjListNode != NULL) {
		// printf("Breakpoint1\n");
		if(currAdjListNode->node_id == node_b_id) {
			if(DEBUG) {
				printf("Edge bewteen node a and node b alreay exists\n");
				printAdjList();
			}
			
			return -3;
		}
		prevAdjListNode = currAdjListNode;
		currAdjListNode = currAdjListNode->next;
	} 
	prevAdjListNode->next = (AdjListNode *)malloc(sizeof(AdjListNode));
	prevAdjListNode = prevAdjListNode->next;
	prevAdjListNode->node_id = node_b_id;
	prevAdjListNode->key = headOfB->key;
	prevAdjListNode->next = NULL;
	if(DEBUG) {
		printf("Completed adding B to A's adjandency list!\n");
	}
	if(DEBUG) {
		printf("Start to add A to B's adjandency list...\n");
	}
	currAdjListNode = headOfB->head;
	prevAdjListNode = currAdjListNode;
	// printf("Breakpoint0\n");
	if(currAdjListNode == NULL) {
		printf("currAdjListNode == NULL. Something wired happens...\n");
		return -3;
	}
	currAdjListNode = currAdjListNode->next;
	while(currAdjListNode != NULL) {
		// printf("Breakpoint1\n");
		if(currAdjListNode->node_id == node_a_id) {
			if(DEBUG) {
				printf("Edge bewteen node a and node b alreay exists\n");
			}
			return -3;
		}
		prevAdjListNode = currAdjListNode;
		currAdjListNode = currAdjListNode->next;
	} 
	prevAdjListNode->next = (AdjListNode *)malloc(sizeof(AdjListNode));
	prevAdjListNode = prevAdjListNode->next;
	prevAdjListNode->node_id = node_a_id;
	prevAdjListNode->key = headOfA->key;
	prevAdjListNode->next = NULL;
	if(DEBUG) { 
		printf("Completed adding A to B's adjandency list!\n");
		printAdjList();
	}
	
	return 0;
}


int add_edge_half(uint64_t node_a_id, uint64_t node_b_id){
	if(DEBUG) {
		printf("Start the process of adding edge between %llu, %llu...\n", node_a_id, node_b_id);
	}
	/* return value:
		1. node_a_id == node_b_id : -1
		2. a or b does not exist: -2
		3. edge between a and b already exists: -3
		4. success: 0
	*/
	if(node_a_id == node_b_id) {
		if(DEBUG) {
			printf("Cannot add edge to the single node %llu!\n", node_a_id);
		}
		return -1;
	}
	AdjList *currAdjList = graph->adjList;
	AdjList *headOfA = NULL;
	if(currAdjList == NULL) {
		printf("curr == NULL!\n");
		return -1;
	}
	currAdjList = currAdjList->next;
	while(currAdjList != NULL) {
		if(currAdjList->node_id == node_a_id) {
			headOfA = currAdjList;
			break;
		}
		currAdjList = currAdjList->next;
	}
	if(headOfA == NULL) {
		if(DEBUG) { 
			printf("NodeA or(and) NodeB does not exists!\n");
			printAdjList();
		}
		return -2;
	}
	// Add edge to A:
	if(DEBUG) {
		printf("Start to add B to A's adjandency list...\n");
	}
	AdjListNode *currAdjListNode = headOfA->head;
	AdjListNode *prevAdjListNode = currAdjListNode;
	// printf("Breakpoint0\n");
	if(currAdjListNode == NULL) {
		if(DEBUG) {
			printf("currAdjListNode == NULL. Something wired happens...\n");
		}
		return -3;
	}
	currAdjListNode = currAdjListNode->next;
	while(currAdjListNode != NULL) {
		// printf("Breakpoint1\n");
		if(currAdjListNode->node_id == node_b_id) {
			if(DEBUG) {
				printf("Edge bewteen node a and node b alreay exists\n");
				printAdjList();
			}
			
			return -3;
		}
		prevAdjListNode = currAdjListNode;
		currAdjListNode = currAdjListNode->next;
	} 
	prevAdjListNode->next = (AdjListNode *)malloc(sizeof(AdjListNode));
	prevAdjListNode = prevAdjListNode->next;
	prevAdjListNode->node_id = node_b_id;
	prevAdjListNode->next = NULL;
	if(DEBUG) {
		printf("Completed adding B to A's adjandency list!\n");
	}
	
	return 0;
}

int remove_edge(uint64_t node_a_id, uint64_t node_b_id){
	if(DEBUG) {
		printf("Start the process of removing edge between %llu, %llu...\n", node_a_id, node_b_id);
	}
	if(node_a_id == node_b_id) {
		if(DEBUG) {
			printf("Edge between %llu and %llu does not exist!\n", node_a_id, node_b_id);
		}
		return -1;
	}
	
	AdjList *currAdjList = graph->adjList;
	AdjList *headOfA = NULL;
	AdjList *headOfB = NULL;
	if(currAdjList == NULL) {
		printf("curr == NULL!\n");
		return -1;
	}
	currAdjList = currAdjList->next;
	while(currAdjList != NULL) {
		if(currAdjList->node_id == node_a_id) {
			headOfA = currAdjList;
		}
		if(currAdjList->node_id == node_b_id) {
			headOfB = currAdjList;
		}
		if(headOfA != NULL && headOfB != NULL) {
			break;
		}
		// printf("*Breakpoint1\n");
		// printf("*Breakpoint2\n");
		currAdjList = currAdjList->next;
	}
	if(headOfA == NULL || headOfB == NULL) {
		if(DEBUG) { 
			printf("NodeA or(and) NodeB does not exists!\n");
			printAdjList();
		}
		
		return -1;
	}
	// Remove edge from A:
	if(DEBUG) {
		printf("Start to remove B from A's adjandency list...\n");
	}
	AdjListNode *currAdjListNode = headOfA->head;
	AdjListNode *prevAdjListNode = currAdjListNode;
	// printf("Breakpoint0\n");
	if(currAdjListNode == NULL) {
		if(DEBUG) {
			printf("currAdjListNode == NULL. Something wired happens...\n");
		}
		return -1;
	}
	currAdjListNode = currAdjListNode->next;
	while(currAdjListNode != NULL) {
		// printf("Breakpoint1\n");
		if(currAdjListNode->node_id == node_b_id) {
			break;
		}
		prevAdjListNode = currAdjListNode;
		currAdjListNode = currAdjListNode->next;
	} 
	if(currAdjListNode == NULL) {
		if(DEBUG) {
			printf("Edge between %llu and %llu does not exist!\n", node_a_id, node_b_id);
		}
		return -1;
	}
	prevAdjListNode->next = currAdjListNode->next;
	if(DEBUG) {
		printf("Completed removing B from A's adjandency list!\n");
	}
	// Remove edge from B:
	if(DEBUG) {
		printf("Start to remove A from B's adjandency list...\n");
	}
	currAdjListNode = headOfB->head;
	prevAdjListNode = currAdjListNode;
	// printf("Breakpoint0\n");
	if(currAdjListNode == NULL) {
		if(DEBUG) {
			printf("currAdjListNode == NULL. Something wired happens...\n");
		}
		return -1;
	}
	currAdjListNode = currAdjListNode->next;
	while(currAdjListNode != NULL) {
		// printf("Breakpoint1\n");
		if(currAdjListNode->node_id == node_a_id) {
			break;
		}
		prevAdjListNode = currAdjListNode;
		currAdjListNode = currAdjListNode->next;
	} 
	if(currAdjListNode == NULL) {
		if(DEBUG) {
			printf("Edge between %llu and %llu does not exist!\n", node_a_id, node_b_id);
		}
		return -1;
	}
	prevAdjListNode->next = currAdjListNode->next;
	if(DEBUG) {
		printf("Completed removing A from B's adjandency list!\n");
		printAdjList();
	}
	
	return 0;
}

int get_edge(uint64_t node_a_id, uint64_t node_b_id){
	if(DEBUG) {
		printf("Start the process of getting edge between %llu, %llu...\n", node_a_id, node_b_id);
	}
	if(node_a_id == node_b_id) {
		if(DEBUG) {
			printf("Edge between %llu and %llu does not exist!\n", node_a_id, node_b_id);
		}
		return -1;
	}
	
	AdjList *currAdjList = graph->adjList;
	AdjList *headOfA = NULL;
	AdjList *headOfB = NULL;
	if(currAdjList == NULL) {
		printf("curr == NULL!\n");
		return -1;
	}
	currAdjList = currAdjList->next;
	while(currAdjList != NULL) {
		if(currAdjList->node_id == node_a_id) {
			headOfA = currAdjList;
		}
		if(currAdjList->node_id == node_b_id) {
			headOfB = currAdjList;
		}
		if(headOfA != NULL && headOfB != NULL) {
			break;
		}
		// printf("*Breakpoint1\n");
		// printf("*Breakpoint2\n");
		currAdjList = currAdjList->next;
	}
	if(headOfA == NULL || headOfB == NULL) {
		if(DEBUG) {
			printf("NodeA or(and) NodeB does not exists!\n");
			printAdjList();
		}
		return -1;
	}
	// get edge from A:
	if(DEBUG) {
		printf("Start to find B in A's adjandency list...\n");
	}
	AdjListNode *currAdjListNode = headOfA->head;
	AdjListNode *prevAdjListNode = currAdjListNode;
	// printf("Breakpoint0\n");
	if(currAdjListNode == NULL) {
		printf("currAdjListNode == NULL. Something wired happens...\n");
		return -1;
	}
	currAdjListNode = currAdjListNode->next;
	while(currAdjListNode != NULL) {
		// printf("Breakpoint1\n");
		if(currAdjListNode->node_id == node_b_id) {
			break;
		}
		prevAdjListNode = currAdjListNode;
		currAdjListNode = currAdjListNode->next;
	} 
	if(currAdjListNode == NULL) {
		if(DEBUG) {
			printf("Edge between %llu and %llu does not exist!\n", node_a_id, node_b_id);
			printAdjList();
		}
		return -1;
	}
	if(DEBUG) {
		printf("Edge between %llu and %llu exists!\n", node_a_id, node_b_id);
		printAdjList();
	}
	return 0;
}

int get_neighbors(uint64_t node_id, AdjListNode **p_neighbors) {
	if(DEBUG) {
		printf("Start the process of getting neighbors of node %llu...\n", node_id);
	}
	// printf("*Breakpoint0\n");
	AdjList *curr = graph->adjList;
	if(curr == NULL) {
		printf("curr == NULL!\n");
		return -1;
	}
	curr = curr->next;
	while(curr != NULL) {
		if(curr->node_id == node_id) {
			if(DEBUG) {
				printf("Node %llu exists!\n", node_id);
				printf("Assign head of curr to neighbors!\n");
			}
			*p_neighbors = curr->head;
			// for (AdjListNode *tmp = *p_neighbors; tmp != NULL; tmp = tmp->next) {
			//        printf("%llu,",tmp->node_id);
			//    }
			if(DEBUG) {
				printAdjList();
			}
			return 0;
		}
		curr = curr->next;
	}
	if(DEBUG) {
		printf("Node %llu does not exist!\n", node_id);
		printAdjList();
	}
	return -1;
}
int shortest_path(uint64_t node_a_id, uint64_t node_b_id) {
	/*
	return code:
	-1: error
	-2: either node does not exist
	>=0 length
	*/
	//find node_a_id and calculate length
	if(DEBUG) {
		printf("Start the process of calculate shortest_path between node %llu and node %llu...\n", node_a_id, node_b_id);
		// printf("*Breakpoint0\n");
		printAdjList();
	}
	if(node_a_id == node_b_id) {
		return 0;
	}
	AdjList *currAdjList = graph->adjList;
	AdjList *headOfA = NULL;
	AdjList *headOfB = NULL;
	if(currAdjList == NULL) {
		printf("curr == NULL!\n");
		return -1;
	}
	currAdjList = currAdjList->next;
	while(currAdjList != NULL) {
		if(currAdjList->node_id == node_a_id) {
			headOfA = currAdjList;
		}
		if(currAdjList->node_id == node_b_id) {
			headOfB = currAdjList;
		}
		// printf("*Breakpoint1\n");
		// printf("*Breakpoint2\n");
		currAdjList = currAdjList->next;
	}
	if(headOfA == NULL || headOfB == NULL) {
		if(DEBUG) {
			printf("NodeA or(and) NodeB does not exists!\n");
			printAdjList();
		}
		return -2;
	}

	int dist[graph->V];
	for(int i = 1; i <= graph->V; i++) {
		dist[i] = -1;
	}
	dist[headOfA->key] = 0;

	int visited[graph->V];
	for(int i = 1; i <= graph->V; i++) {
		visited[i] = 0;
	}

	uint64_t p = headOfA->key;
	AdjList *headOfP = NULL;

	// printf("Breakpoint0\n");
	while(p != headOfB->key) {
		visited[p] = 1;
		currAdjList = graph->adjList;
		while(currAdjList != NULL) {
			if(currAdjList->key == p) {
				headOfP = currAdjList;
				break;
			}
			currAdjList = currAdjList->next;
		} 
		// printf("Breakpoint1 %llu\n", headOfP->key);
		AdjListNode *tmp = headOfP->head;
		if(tmp == NULL) {
			printf("Error\n");
		}
		tmp = tmp->next;
		while(tmp != NULL) {
			if(visited[tmp->key] == 0 && (dist[tmp->key] == -1 || dist[tmp->key] > dist[headOfP->key] + 1)) {
				// printf("Breakpoint2: %llu\n", tmp->key);
				dist[tmp->key] = dist[headOfP->key] + 1;
			}
			tmp = tmp->next;
		}
		int min = graph->V + 1;
		int minKey = -1;
		for(int i = 1; i <= graph->V; i++) {
			if(visited[i] == 0 && dist[i] != -1 && dist[i] < min) {
				min = dist[i];
				minKey = i;
			}
		}
		p = minKey;
		// printf("Breakpoint3: %llu\n", p);
		if(minKey == -1) {
			return -1;
		}
	}
	// printf("%d\n", graph->V);

	// for(int i = 0; i <= graph->V; i++) {
	// 	printf("%d\n", dist[i]);
	// }
	return dist[headOfB->key];
}
void printAdjList() {
	// temp 
	return;
	printf("*************************************\n");
	printf("The current state of adjandency list: \n");
	AdjList *curr = graph->adjList;
	while(curr != NULL) {
		printf("%llu(%llu)", curr->node_id, curr->key);
		AdjListNode *tmp = curr->head;
		while(tmp != NULL) {
			printf(":%llu(%llu) ", tmp->node_id, tmp->key);
			tmp = tmp->next;
		}
		printf("\n");
		curr = curr->next;
	}
	printf("*************************************\n");
}
// int main() {
// 	graph_init();
// 	for(int i = 0; i < 5; i++) {
// 		add_node(i);
// 	}
// 	printAdjList();
// }