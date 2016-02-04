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

static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;

const int DEBUG = 0;
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
            break;
        default:
            break;
    }
}

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
    struct json_token *json_arr = parse_json2(hm->body.p, hm->body.len); //json_token指针 是把所有json 连着存的
    if(DEBUG) {
        printf("\n####### Handling get neighbors #######\n");
        printf("The requset json is: %s\n", json_arr->ptr);
    }
    
    // extract node id from json
    struct json_token *node_id = find_json_token(json_arr, "node_id");
    free(json_arr);
    
    char buf[100];
    snprintf(buf, 100, "%.*s\n", node_id->len, node_id->ptr);
    
    int id = atoi(buf);
    AdjListNode *neighbors;
    int rc = get_neighbors(id, &neighbors);
    char data[100];
    if (rc == 0) {
        snprintf(data, 100, "{ \"node_id\": %d, \"neighbors\": [", id);
        int flag = 0;
        for (AdjListNode *tmp = neighbors->next; tmp != NULL; tmp = tmp->next) {
            snprintf(data, 100, "%s%llu,", data, tmp->node_id);
            flag = 1;
        }
        // if flag == 0, means it has no neighbout, no need to delete the last ','
        if(flag) {
            data[strlen(data) - 1] = '\0';
        }   
        snprintf(data, 100, "%s] }", data);
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

void handle_shortest_path(struct mg_connection *nc, struct http_message *hm) {
    struct json_token *json_arr = parse_json2(hm->body.p, hm->body.len); //json_token指针 是把所有json 连着存的
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

int main(int argc, char *argv[]) {
    if(DEBUG) {
        printf("DEBUG MODE OPEN\n");
    }
    else {
        printf("DEBUG MODE CLOSED\n");
    }
    struct mg_mgr mgr;
    struct mg_connection *nc;
    int i;
    char *cp;
    
    mg_mgr_init(&mgr, NULL);
    
    graph_init();
    
    /* Process command line options to customize HTTP server */
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-D") == 0 && i + 1 < argc) {
            mgr.hexdump_file = argv[++i];
        } else if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
            s_http_server_opts.document_root = argv[++i];
        } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            s_http_port = argv[++i];
        } else if (strcmp(argv[i], "-a") == 0 && i + 1 < argc) {
            s_http_server_opts.auth_domain = argv[++i];
        } else if (strcmp(argv[i], "-P") == 0 && i + 1 < argc) {
            s_http_server_opts.global_auth_file = argv[++i];
        } else if (strcmp(argv[i], "-A") == 0 && i + 1 < argc) {
            s_http_server_opts.per_directory_auth_file = argv[++i];
        } else if (strcmp(argv[i], "-r") == 0 && i + 1 < argc) {
            s_http_server_opts.url_rewrites = argv[++i];
        } else {
            fprintf(stderr, "Unknown option: [%s]\n", argv[i]);
            exit(1);
        }
    }
    
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


