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

#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/TToString.h>

#include <boost/make_shared.hpp>

#include <iostream>
#include <stdexcept>
#include <sstream>
#include <functional>
#include <thread>
#include <cstring>

#include "gen-cpp/RemoteService.h"

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

using namespace remote;
using namespace std;

static struct mg_serve_http_opts s_http_server_opts;

const int DEBUG = 0;

static char s_http_port[20];

int partitionIndex;
vector<string> partitions;

// local server
char s_next_host[20];
char s_next_http_port[20];

// partition servers
string s_http_hosts[20];
string s_http_ports[20];
int partitionCount;

int get_rpc_port(int partitionIdx) {
    return partitionIdx + 20000;
}

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

void init() {
    // if (!tailFlag) {
    //     boost::shared_ptr<TTransport> socket(new TSocket(s_next_host, next_rpc_port));
    //     boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    //     boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    //     client = new RemoteServiceClient(protocol);
    //     transport->open();
    // }
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
    // if (!tailFlag) {
    //     boost::shared_ptr<TTransport> socket(new TSocket(s_next_host, next_rpc_port));
    //     boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    //     boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    //     RemoteServiceClient client(protocol);
    //     transport->open();
    //     client.rmt_add_node(id);
    //     transport->close();
    // }
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

    // if (!tailFlag) {
    //     boost::shared_ptr<TTransport> socket(new TSocket(s_next_host, next_rpc_port));
    //     boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    //     boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    //     RemoteServiceClient client(protocol);
    //     transport->open();
    //     client.rmt_remove_node(id);
    //     transport->close();
    // }
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
    
    const char *data;
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
    // if (!tailFlag) {
    //     boost::shared_ptr<TTransport> socket(new TSocket(s_next_host, next_rpc_port));
    //     boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    //     boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    //     RemoteServiceClient client(protocol);
    //     transport->open();
    //     client.rmt_add_edge(a_id, b_id);
    //     transport->close();
    // }
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
    // if (!tailFlag) {
    //     boost::shared_ptr<TTransport> socket(new TSocket(s_next_host, next_rpc_port));
    //     boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    //     boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    //     RemoteServiceClient client(protocol);
    //     transport->open();
    //     client.rmt_remove_edge(a_id, b_id);
    //     transport->close();
    // }
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

    const char *data = NULL;
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

class RemoteServiceHandler : virtual public RemoteServiceIf {
 public:
  RemoteServiceHandler() {}

  int32_t rmt_add_node(const int64_t node_id) {
    // Your implementation goes here
    int rc = add_node(node_id);
    // if (!tailFlag) {
    //     boost::shared_ptr<TTransport> socket(new TSocket(s_next_host, next_rpc_port));
    //     boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    //     boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    //     RemoteServiceClient client(protocol);
    //     transport->open();
    //     client.rmt_add_node(node_id);
    //     transport->close();
    // }
    return rc;
    // printf("rmt_add_node\n");
  }

  int32_t rmt_add_edge(const int64_t node_a_id, const int64_t node_b_id) {
    // Your implementation goes here
    int rc = add_edge(node_a_id, node_b_id);
    // if (!tailFlag) {
    //     boost::shared_ptr<TTransport> socket(new TSocket(s_next_host, next_rpc_port));
    //     boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    //     boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    //     RemoteServiceClient client(protocol);
    //     transport->open();
    //     client.rmt_add_edge(node_a_id, node_b_id);
    //     transport->close();
    // }
    return rc;
    // printf("rmt_add_edge\n");
  }

  int32_t rmt_remove_node(const int64_t node_id) {
    // Your implementation goes here
    int rc = remove_node(node_id);
    // if (!tailFlag) {
    //     boost::shared_ptr<TTransport> socket(new TSocket(s_next_host, next_rpc_port));
    //     boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    //     boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    //     RemoteServiceClient client(protocol);
    //     transport->open();
    //     client.rmt_remove_node(node_id);
    //     transport->close();
    // }
    return rc;
    // printf("rmt_remove_node\n");
  }

  int32_t rmt_remove_edge(const int64_t node_a_id, const int64_t node_b_id) {
    // Your implementation goes here
    int rc = remove_edge(node_a_id, node_b_id);
    // if (!tailFlag) {
    //     boost::shared_ptr<TTransport> socket(new TSocket(s_next_host, next_rpc_port));
    //     boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    //     boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    //     RemoteServiceClient client(protocol);
    //     transport->open();
    //     client.rmt_remove_edge(node_a_id, node_b_id);
    //     transport->close();
    // }
    return rc;
    // printf("rmt_remove_edge\n");
  }

};

void runMongoose() {
    struct mg_mgr mgr;
    struct mg_connection *nc;
    
    mg_mgr_init(&mgr, NULL);
    
    init();
    graph_init();
    
    /* Set HTTP server options */
    nc = mg_bind(&mgr, s_http_port, ev_handler);
    
    if (nc == NULL) {
        fprintf(stderr, "Error starting server on port %s\n", s_http_port);
        exit(1);
    }
    
    mg_set_protocol_http_websocket(nc);
    s_http_server_opts.document_root = ".";
    s_http_server_opts.enable_directory_listing = "yes";

    printf("Starting mongoose server on port %s\n", s_http_port);
    for (;;) {
        mg_mgr_poll(&mgr, 1000);
    }
    mg_mgr_free(&mgr);
}

void runThrift() {
    boost::shared_ptr<RemoteServiceHandler> handler(new RemoteServiceHandler());
    boost::shared_ptr<TProcessor> processor(new RemoteServiceProcessor(handler));
    boost::shared_ptr<TServerTransport> serverTransport(new TServerSocket(get_rpc_port(partitionIndex)));
    boost::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
    boost::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

    TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
    printf("Starting thrift server on port %d\n", get_rpc_port(partitionIndex));
    server.serve();
}

vector<string> split(string str, char delimiter) {
  vector<string> internal;
  stringstream ss(str); // Turn the string into a stream.
  string tok;
  
  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }
  
  return internal;
}

int atoi(string s) {
  int rt;
  stringstream ss(s);
  ss >> rt;
  return rt;
}

int main(int argc, char *argv[]) {
    if(DEBUG) {
        printf("DEBUG MODE OPEN\n");
    }
    else {
        printf("DEBUG MODE CLOSED\n");
    }

    extern char *optarg;
    int c;
    // http server port
    strcpy(s_http_port, argv[1]);
    argv++;
    while ((c = getopt(argc - 1, argv, "p:l:")) != -1) {
        if(optarg != NULL) {
            string s(optarg);
            vector<string> v = split(s, ' ');
            int i = 0;
            switch (c) {
                // localhost
                case 'p':
                    partitionIndex = atoi(v[0]);
                    s_http_hosts[partitionIndex] = "localhost";
                    s_http_ports[partitionIndex] = s_http_port;
                    break;
                // next in chain
                case 'l':
                    for (vector<string>::iterator it = v.begin() ; it != v.end(); ++it) {
                        // cout << *it << endl;
                        // skip self as servers are in order
                        if (i == partitionIndex)
                            i++;
                        vector<string> tmp = split(*it, ':');
                        s_http_hosts[i] = tmp[0];
                        s_http_ports[i] = tmp[1];
                        i ++;
                    }
                    partitionCount = i;
                    break;
                default:
                    break;
            }
        }
        else {
            printf("optarg is null!!\n");
        }   
    }

    cout << "This node: localhost...port(" << s_http_port << "), rpc port(" << get_rpc_port(partitionIndex) << ")" << endl;

    for (int i = 0; i < partitionCount; i ++) {
        cout << "partition " << i << ":" << s_http_hosts[i] << "...port(" << s_http_ports[i] << "), rpc port(" << get_rpc_port(i) << ")" << endl;
    }

    std::thread mongooseThread(runMongoose);
    mongooseThread.detach();
    std::thread thriftThread(runThrift);
    thriftThread.detach();

    while (1) {}
    
    return 0;
}


