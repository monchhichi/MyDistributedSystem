# My graph server
Distributed Graph Server for cs426.


## Interfaces

We're using a simple HTTP based system, which only uses the `HTTP` and `Content-Length` headers. All requests will be in the form:
	
	HTTP
	POST <function_name> HTTP/1.0
	<other-headers?>
	Content-Length: <length>
	<other-headers?>
	

<length bytes of JSON encoded content>


The arguments will be in [JSON](http://www.json.org/). You can use the [Mongoose Library](https://github.com/cesanta/mongoose) in C or C++ for both request handling and JSON decoding if you so desire to. Seach for JSON and HTTP in the header file for more information.


In this lab you will implement the following functions for your undirected-graph store:

   Function    | Method |    Arguments     | Return
-------------- | ------ | ---------------- | ------
 `add_node`    | `POST` | `u64 node_id`    |  `200` on success<br/> `204`if the node already exists
 `add_edge`    | `POST` | `u64 node_a_id`, `u64 node_b_id`  |  `200` on success<br/>`204` if the edge already exists<br /> `400` if either node doesn't exist, or if `node_a_id` is the same as `node_b_id`
 `remove_node` | `POST` | `u64 node_id` | `200` on success<br/> `400`if the node does not exist
 `remove_edge` | `POST` | `u64 node_a_id`, `u64 node_b_id`  |  `200` on success<br/>`400` if the edge does not exist
 `get_node`    | `POST` | `u64 node_id` | `200` and a boolean JSON field `in_graph` indicating whether the node is in the graph
 `get_edge`    | `POST` | `u64 node_a_id`, `u64 node_b_id`  |  `200` and a boolean JSON field `in_graph` indicating whether the node is in the graph<br/>`400` of at least one of the vertices does not exist
 `get_neighbors`   | `POST`  | `u64 node_id` | `200` and a list of neighbors[*](#get_neighbors_description) or<br/> `400` if the node does not exist
 `shortest_path`   | `POST`  | `u64 node_a_id`, `uint node_b_id` | `u64` and a field `distance` containing the shortest path between the two nodes or<br/>`204` if there is no path <br/>`400` if either node does not exist
`checkpoint`    | `POST` |     |  `200` on success<br/> `507`if there is insufficient space for the checkpoint
`restore`    | `POST` |     |  `200` on success<br/> `507`if failed

<a name="get_neighbors_description">*</a> Specifically the contents returned from `get_neighbors` must be in the form:
	
	{
  		"node_id": <node_id>,
  		"neighbors": [<neighbors>]
	}

## On-Disk Log format

The on-disk format is as follows.
### Super block
The first block is a superblock for storing metadata.
`byte 0: 32 bits unsigned int: start of log segment` (In my code should be block 1)<br/>
`byte 4: 32 bits unsigned int: size of log segment` (In my code should be 0.5 * 1024 * 1024 blocks)<br/>
`byte 8: 32 bits unsigned int: current head of the useful log` (e.g. block 55)<br/>
`byte 12: 32 bits unsigned int: current tail of the useful log` (e.g. block 55)<br/>
`byte 16: 32 bits unsigned int: newest generation of the log` (e.g. 10)<br/>

### Log block segment
Here stores all the logs(useful and non-useful).
`byte 0: 32 bits unsigned int: generation number`<br/>
`byte 4: 32 bits unsigned int: number of entries in block`<br/>
`byte 8: first 20-byte entry`<br/>
`byte 28: second 20-byte entry`<br/>
`...`

The log entry format is as follows:

Each log entry will have a 4-byte opcode (0 for `add_node`, 1 for `add_edge`, 2 for `remove_node`, 3 for `remove_edge`) and two 64-bit node IDs (only one of which is used for `add_node` and `remove_node`).


### Checkpoint block segment

The remainder of the disk is used for the checkpoint. 

