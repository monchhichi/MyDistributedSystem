void ev_handler(struct mg_connection *nc, int ev, void *ev_data);
void handle_add_node(struct mg_connection *nc, struct http_message *hm);
void handle_add_edge(struct mg_connection *nc, struct http_message *hm);
void handle_remove_node(struct mg_connection *nc, struct http_message *hm);
void handle_get_node(struct mg_connection *nc, struct http_message *hm);
void handle_get_edge(struct mg_connection *nc, struct http_message *hm);
void handle_get_neighbors(struct mg_connection *nc, struct http_message *hm);
void handle_shortest_path(struct mg_connection *nc, struct http_message *hm);
void handle_remove_edge(struct mg_connection *nc, struct http_message *hm);