//
//  main.c
//  cs426
//
//  Created by Yiyuan on 2/4/16.
//  Copyright (c) 2016 monchhichi. All rights reserved.
//

#include <stdlib.h>
#include "mongoose.h"
#include "graph.h"
#include "server.h"
#include "log.h"

static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;
char *log_filename;

const int DEBUG = 1;
void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
    struct http_message *hm = (struct http_message *) ev_data;
    
    switch (ev) {
        case MG_EV_HTTP_REQUEST:
            if (mg_vcmp(&hm->uri, "/api/v1/add_node") == 0) {
                handle_add_node(nc, hm);
            }
            else if (mg_vcmp(&hm->uri, "/api/v1/add_edge") == 0) {
                handle_add_edge(nc, hm);
            }
            else if (mg_vcmp(&hm->uri, "/api/v1/remove_node") == 0) {
                handle_remove_node(nc, hm);
            }
            else if (mg_vcmp(&hm->uri, "/api/v1/remove_edge") == 0) {
                handle_remove_edge(nc, hm);
            }
            else if (mg_vcmp(&hm->uri, "/api/v1/get_node") == 0) {
                handle_get_node(nc, hm);
            }
            else if (mg_vcmp(&hm->uri, "/api/v1/get_edge") == 0) {
                handle_get_edge(nc, hm);
            }
            else if (mg_vcmp(&hm->uri, "/api/v1/get_neighbors") == 0) {
                handle_get_neighbors(nc, hm);
            }
            else if (mg_vcmp(&hm->uri, "/api/v1/shortest_path") == 0) {
                handle_shortest_path(nc, hm);
            }
            else if(mg_vcmp(&hm->uri, "/api/v1/checkpoint") == 0) {
                handle_check_point(nc, hm);
            }
            else if(mg_vcmp(&hm->uri, "/api/v1/restore") == 0) {
                handle_restore(nc, hm);
            }
            break;
        default:
            break;
    }
}
// Each log entry will have a 4-byte opcode (0 for add_node, 1 for add_edge, 2 for remove_node, 3 for remove_edge) 
// and two 64-bit node IDs (only one of which is used for add_node and remove_node).
void handle_add_node(struct mg_connection *nc, struct http_message *hm) {
    // parse json
    struct json_token *json_arr = parse_json2(hm->body.p, hm->body.len); //json_token指针 是把所有json 连着存的

    if(DEBUG) {
        printf("\n####### Handling add node #######\n");
        printf("The requset json is: %s\n", json_arr->ptr);
    }
    
    // extract node id from json
    struct json_token *node_id = find_json_token(json_arr, "node_id");
    free(json_arr);
    
    char buf[100], data[100];
    snprintf(buf, 100, "%.*s\n", node_id->len, node_id->ptr);
    
    int id = atoi(buf);

    // wirte to log
    int rc0 = add_log(OP_ADD_NODE, id, 0);
    if (rc0 == -1) {
        mg_printf(nc, "%s", "HTTP/1.1 507 Insufficient Storage\r\nContent-Length: 0\r\n\r\n");
        return;
    }

    int rc = add_node(id);
    
    if (rc == 0) {
        snprintf(data, 20, "{ \"node_id\": %d }", id);
        mg_printf(nc, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n"
                  "Content-Type: application/json\r\n\r\n%s",
                  (int) strlen(data), data);
    }
    else if(rc == -1) {
        mg_printf(nc, "%s", "HTTP/1.1 204 OK\r\nContent-Length: 0\r\n\r\n");
    }
    else {
        printf("Unknown return value!\n");
    }


}

void handle_remove_node(struct mg_connection *nc, struct http_message *hm) {
    // parse json
    struct json_token *json_arr = parse_json2(hm->body.p, hm->body.len); //json_token指针 是把所有json 连着存的
    if(DEBUG) {
        printf("\n####### Handling remove node #######\n");
        printf("The requset json is: %s\n", json_arr->ptr);
    }
    
    // extract node id from json
    struct json_token *node_id = find_json_token(json_arr, "node_id");
    free(json_arr);
    
    char buf[100], data[100];
    snprintf(buf, 100, "%.*s\n", node_id->len, node_id->ptr);
    
    int id = atoi(buf);
    
    // wirte to log
    int rc0 = add_log(OP_REMOVE_NODE, id, 0);
    if (rc0 == -1) {
        mg_printf(nc, "%s", "HTTP/1.1 507 Insufficient Storage\r\nContent-Length: 0\r\n\r\n");
        return;
    }

    int rc = remove_node(id);
    
    if (rc == 0) {
        snprintf(data, 100, "{ \"node_id\": %d }", id);
        mg_printf(nc, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n"
                  "Content-Type: application/json\r\n\r\n%s",
                  (int) strlen(data), data);
    }
    else if(rc == -1){
        mg_printf(nc, "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n");
    }
    else {
        printf("Unknown return value!\n");
    }
}

void handle_get_node(struct mg_connection *nc, struct http_message *hm) {
    // parse json
    struct json_token *json_arr = parse_json2(hm->body.p, hm->body.len); //json_token指针 是把所有json 连着存的
    if(DEBUG) {
        printf("\n####### Handling get node #######\n");
        printf("The requset json is: %s\n", json_arr->ptr);
    }
    
    // extract node id from json
    struct json_token *node_id = find_json_token(json_arr, "node_id");
    free(json_arr);
    
    char buf[100];
    snprintf(buf, 100, "%.*s\n", node_id->len, node_id->ptr);
    
    int id = atoi(buf);
    int rc = get_node(id);
    
    char *data;
    data = (rc == 0)?  "{ \"in_graph\": true }" : "{ \"in_graph\": false }";
    
    mg_printf(nc, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n"
              "Content-Type: application/json\r\n\r\n%s",
              (int) strlen(data), data);
}

void handle_add_edge(struct mg_connection *nc, struct http_message *hm) {
    // parse json
    struct json_token *json_arr = parse_json2(hm->body.p, hm->body.len); //json_token指针 是把所有json 连着存的
    if(DEBUG) {
        printf("\n####### Handling add edge #######\n");
        printf("The requset json is: %s\n", json_arr->ptr);
    }
    // extract node id from json
    struct json_token *node_a_id = find_json_token(json_arr, "node_a_id");
    struct json_token *node_b_id = find_json_token(json_arr, "node_b_id");
    
    free(json_arr);
    
    char buf[100];
    snprintf(buf, 100, "%.*s\n", node_a_id->len, node_a_id->ptr);
    int a_id = atoi(buf);
    snprintf(buf, 100, "%.*s\n", node_b_id->len, node_b_id->ptr);
    int b_id = atoi(buf);

    // write to log
    int rc0 = add_log(OP_ADD_EDGE, a_id, b_id);
    if (rc0 == -1) {
        mg_printf(nc, "%s", "HTTP/1.1 507 Insufficient Storage\r\nContent-Length: 0\r\n\r\n");
        return;
    }


    int rc = add_edge(a_id, b_id);
    char data[100];
    if (rc == 0) {
        snprintf(data, 100, "{ \"node_a_id\": %d, \"node_b_id\": %d }", a_id, b_id);
        
        mg_printf(nc, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n"
                  "Content-Type: application/json\r\n\r\n%s",
                  (int) strlen(data), data);
    }
    else if (rc == -1 || rc == -2) {
        mg_printf(nc, "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n");
    }
    else if (rc == -3){
        mg_printf(nc, "HTTP/1.1 204 OK\r\nContent-Length: 0\r\n\r\n");
    }
    else {
        printf("Unknown return value!\n");
    }
}

void handle_remove_edge(struct mg_connection *nc, struct http_message *hm) {
    // parse json
    struct json_token *json_arr = parse_json2(hm->body.p, hm->body.len); //json_token指针 是把所有json 连着存的
    if(DEBUG) {
        printf("\n####### Handling remove edge #######\n");
        printf("The requset json is: %s\n", json_arr->ptr);
    }
    // extract node id from json
    struct json_token *node_a_id = find_json_token(json_arr, "node_a_id");
    struct json_token *node_b_id = find_json_token(json_arr, "node_b_id");
    
    free(json_arr);
    
    char buf[100];
    snprintf(buf, 100, "%.*s\n", node_a_id->len, node_a_id->ptr);
    int a_id = atoi(buf);
    snprintf(buf, 100, "%.*s\n", node_b_id->len, node_b_id->ptr);
    int b_id = atoi(buf);

    // write to log
    int rc0 = add_log(OP_REMOVE_EDGE, a_id, b_id);
    if (rc0 == -1) {
        mg_printf(nc, "%s", "HTTP/1.1 507 Insufficient Storage\r\nContent-Length: 0\r\n\r\n");
        return;
    }
    
    int rc = remove_edge(a_id, b_id);
    char data[100];
    if (rc == 0) {
        snprintf(data, 100, "{ \"node_a_id\": %d, \"node_b_id\": %d }", a_id, b_id);

        mg_printf(nc, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n"
                "Content-Type: application/json\r\n\r\n%s",
                (int) strlen(data), data);
    } 
    else if(rc == -1) {
        mg_printf(nc, "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n");
    }
    else {
        printf("Unknown return value!\n");
    }
}

void handle_get_edge(struct mg_connection *nc, struct http_message *hm) {
    // parse json
    struct json_token *json_arr = parse_json2(hm->body.p, hm->body.len); //json_token指针 是把所有json 连着存的
    if(DEBUG) {
        printf("\n####### Handling get edge #######\n");
        printf("The requset json is: %s\n", json_arr->ptr);
    }
    
    // extract node id from json
    struct json_token *node_a_id = find_json_token(json_arr, "node_a_id");
    struct json_token *node_b_id = find_json_token(json_arr, "node_b_id");
    
    free(json_arr);
    
    char buf[100];
    snprintf(buf, 100, "%.*s\n", node_a_id->len, node_a_id->ptr);
    int a_id = atoi(buf);
    snprintf(buf, 100, "%.*s\n", node_b_id->len, node_b_id->ptr);
    int b_id = atoi(buf);
    int rc = get_edge(a_id, b_id);

    char *data = NULL;
    if (rc == 0) {
        data = "{ \"in_graph\": true }";
    }
    else if(rc == -1) {
        data = "{ \"in_graph\": false }";
    }
    else {
        printf("Unknown return value!\n");
    }
    mg_printf(nc, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n"
                "Content-Type: application/json\r\n\r\n%s",
                (int) strlen(data), data);
}

void handle_get_neighbors(struct mg_connection *nc, struct http_message *hm) {
    // parse json
    struct json_token *json_arr = parse_json2(hm->body.p, hm->body.len); 
    if(DEBUG) {
        printf("\n####### Handling get neighbors #######\n");
        printf("The requset json is: %s\n", json_arr->ptr);
    }
    
    // extract node id from json
    struct json_token *node_id = find_json_token(json_arr, "node_id");
    free(json_arr);
    
    char buf[1000];
    snprintf(buf, 1000, "%.*s\n", node_id->len, node_id->ptr);
    
    int id = atoi(buf);
    AdjListNode *neighbors;
    int rc = get_neighbors(id, &neighbors);
    char data[1000];
    if (rc == 0) {
        snprintf(data, 1000, "{ \"node_id\": %d, \"neighbors\": [", id);
        int flag = 0;
        char temp[1000];
        // printf("previous: %s\n", data);
        for (AdjListNode *tmp = neighbors->next; tmp != NULL; tmp = tmp->next) {
            // snprintf(tmp, 1000, "%llu,", tmp->node_id);
            snprintf(temp, 1000, "%llu", tmp->node_id);
            // printf("temp: %s", temp);
            strcat(data, temp);
            strcat(data, ",");
            // printf("current data%s\n", data);
            flag = 1;
        }
        // if flag == 0, means it has no neighbors, no need to delete the last ','
        if(flag) {
            data[strlen(data) - 1] = '\0';
        }   
        // printf("data: %s\n", temp);
        // strcat(data, temp);
        strcat(data, "] }");
        // printf("after: %s\n", data);

        // snprintf(data, 1000, "%s] }", data);
        mg_printf(nc, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n"
                    "Content-Type: application/json\r\n\r\n%s",
                    (int) strlen(data), data);
        // printf("%.*s\n", nc->send_mbuf.len, nc->send_mbuf.buf);

    } 
    else if(rc == -1){
        mg_printf(nc, "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n");
        // printf("%.*s\n", nc->send_mbuf.len, nc->send_mbuf.buf);
    }
    else {
        printf("Unknown return value!\n");
    }
}

void handle_shortest_path(struct mg_connection *nc, struct http_message *hm) {
    struct json_token *json_arr = parse_json2(hm->body.p, hm->body.len);
    if(DEBUG) {
        printf("\n####### Handling caculate shortest_path #######\n");
        printf("The requset json is: %s\n", json_arr->ptr);
    }
    
    // extract node id from json
    struct json_token *node_a_id = find_json_token(json_arr, "node_a_id");
    struct json_token *node_b_id = find_json_token(json_arr, "node_b_id");
    
    free(json_arr);
    
    char buf[100];
    snprintf(buf, 100, "%.*s\n", node_a_id->len, node_a_id->ptr);
    int a_id = atoi(buf);
    snprintf(buf, 100, "%.*s\n", node_b_id->len, node_b_id->ptr);
    int b_id = atoi(buf);
    int rc = shortest_path(a_id, b_id);
    char data[100];
    /*
    return code:
    -1: error
    -2: either node does not exist
    >=0 length
    */
    if(rc >= 0) {
        snprintf(data, 100, "{ \"distance\": %d }", rc);

        mg_printf(nc, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n"
                "Content-Type: application/json\r\n\r\n%s",
                (int) strlen(data), data);
    }
    else if(rc == -2) {
        mg_printf(nc, "HTTP/1.1 204 OK\r\nContent-Length: 0\r\n\r\n"); 
    }
    else if(rc == -1) {
        mg_printf(nc, "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n");
    }
    else {
        printf("Unknown return value!\n");
    }
}

void handle_check_point(struct mg_connection *nc, struct http_message *hm) {
    if(DEBUG) {
        printf("\n####### Handling adding checkpoint #######\n");
    }
    int rc = checkpoint();
    if(rc == 0) {
        mg_printf(nc, "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n");
    }
    else if(rc == -1) {
        mg_printf(nc, "HTTP/1.1 507 Insufficient Space\r\nContent-Length: 0\r\n\r\n");
    }
    else {
        printf("Unknown return value!\n");
    }
}

void handle_restore(struct mg_connection *nc, struct http_message *hm) {
    if(DEBUG) {
        printf("\n####### Handling restoring checkpoint #######\n");
    }
    int rc = restore();
    if(rc == 0) {
        mg_printf(nc, "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n");
    }
    else if(rc == -1) {
        mg_printf(nc, "HTTP/1.1 507 Failed\r\nContent-Length: 0\r\n\r\n");
    }
    else {
        printf("Unknown return value!\n");
    }
}

int main(int argc, char *argv[]) {
    if(DEBUG) {
        printf("DEBUG MODE OPEN\n");
    }
    else {
        printf("DEBUG MODE CLOSED\n");
    }
    
    graph_init();
    if (argc == 4) {
        s_http_port = argv[2];
        log_filename = argv[3];
        if(!strcmp(argv[1], "-f")) {
            format_disk();
        }
    }
    else if(argc == 3){
        s_http_port = argv[1];
        log_filename = argv[2];
        restore();
    }
    else {
        printf("INPUT FORMAT ERROR!!\n");
        printf("Correct input format:\n");
        printf("./cs426_graph_server <port> <devfile>\n");
        printf("$ ./cs426_graph_server -f <port> <devfile>\n");
        return 0;
    }
    struct mg_mgr mgr;
    struct mg_connection *nc;
    char *cp;
    
    mg_mgr_init(&mgr, NULL);
    
    /* Set HTTP server options */
    nc = mg_bind(&mgr, s_http_port, ev_handler);
    if (nc == NULL) {
        fprintf(stderr, "Error starting server on port %s\n", s_http_port);
        exit(1);
    }
    mg_set_protocol_http_websocket(nc);
    s_http_server_opts.document_root = ".";
    s_http_server_opts.enable_directory_listing = "yes";
    /* Use current binary directory as document root */
    if (argc > 0 && ((cp = strrchr(argv[0], '/')) != NULL ||
                     (cp = strrchr(argv[0], '/')) != NULL)) {
        *cp = '\0';
        s_http_server_opts.document_root = argv[0];
    }
    
    printf("Starting server on port %s\n", s_http_port);
    for (;;) {
        mg_mgr_poll(&mgr, 1000);
    }
    mg_mgr_free(&mgr);
    return 0;
}


