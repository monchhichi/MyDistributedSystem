// A structure to represent an adjacency list node
typedef struct AdjListNode
{
    uint64_t node_id;
    uint64_t key;
    struct AdjListNode* next;
}AdjListNode;
 
// A structure to represent an adjacency list
typedef struct AdjList
{
	uint64_t node_id;
	uint64_t key;
    struct AdjListNode *head;  // pointer to head node of list
    struct AdjList *next;
}AdjList;
 
// A structure to represent a graph. A graph is an array of adjacency lists.
// Size of array will be V (number of vertices in graph)
typedef struct Graph
{
    int V;
    struct AdjList* adjList;
}Graph;

void graph_init();
int add_node(uint64_t node_id);
int remove_node(uint64_t node_id);
int get_node(uint64_t node_id);
int get_edge(uint64_t node_a_id, uint64_t node_b_id);
int add_edge(uint64_t node_a_id, uint64_t node_b_id);
int remove_edge(uint64_t node_a_id, uint64_t node_b_id);
int get_neighbors(uint64_t node_id, AdjListNode **p_neighbors);
int shortest_path(uint64_t node_a_id, uint64_t node_b_id);
void printAdjList();