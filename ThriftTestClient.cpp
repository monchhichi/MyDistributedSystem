#include <iostream>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "gen-cpp/RemoteService.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace remote;

int main() {
  boost::shared_ptr<TTransport> socket(new TSocket("localhost", 9000));
  boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  RemoteServiceClient client(protocol);

  try {
    transport->open();
    
    cout << "add_node(101)" << client.rmt_add_node(101) << endl;
    cout << "get_node(101)" << client.rmt_get_node(101) << endl;
    cout << "get_node(102)" << client.rmt_get_node(102) << endl;

    transport->close();
  } catch (TException& tx) {
    cout << "ERROR: " << tx.what() << endl;
  }
}