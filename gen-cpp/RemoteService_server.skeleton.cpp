// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "RemoteService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace  ::remote;

class RemoteServiceHandler : virtual public RemoteServiceIf {
 public:
  RemoteServiceHandler() {
    // Your initialization goes here
  }

  /**
   * A method definition looks like C code. It has a return type, arguments,
   * and optionally a list of exceptions that it may throw. Note that argument
   * lists and exception lists are specified using the exact same syntax as
   * field lists in struct or exception definitions.
   * 
   * @param node_a_id
   * @param node_b_id
   */
  int32_t rmt_add_edge_half(const int64_t node_a_id, const int64_t node_b_id) {
    // Your implementation goes here
    printf("rmt_add_edge_half\n");
  }

  int32_t rmt_remove_edge(const int64_t node_a_id, const int64_t node_b_id) {
    // Your implementation goes here
    printf("rmt_remove_edge\n");
  }

  int32_t rmt_remove_edge_half(const int64_t node_a_id, const int64_t node_b_id) {
    // Your implementation goes here
    printf("rmt_remove_edge_half\n");
  }

};

int main(int argc, char **argv) {
  int port = 9090;
  shared_ptr<RemoteServiceHandler> handler(new RemoteServiceHandler());
  shared_ptr<TProcessor> processor(new RemoteServiceProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}

