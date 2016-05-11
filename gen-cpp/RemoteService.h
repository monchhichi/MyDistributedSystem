/**
 * Autogenerated by Thrift Compiler (0.9.3)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef RemoteService_H
#define RemoteService_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "RemoteService_types.h"

namespace remote {

#ifdef _WIN32
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class RemoteServiceIf {
 public:
  virtual ~RemoteServiceIf() {}

  /**
   * A method definition looks like C code. It has a return type, arguments,
   * and optionally a list of exceptions that it may throw. Note that argument
   * lists and exception lists are specified using the exact same syntax as
   * field lists in struct or exception definitions.
   * 
   * @param node_id
   */
  virtual int32_t rmt_add_node(const int64_t node_id) = 0;
  virtual int32_t rmt_add_edge_half(const int64_t node_a_id, const int64_t node_b_id) = 0;
  virtual int32_t rmt_remove_node(const int64_t node_id) = 0;
  virtual int32_t rmt_remove_edge(const int64_t node_a_id, const int64_t node_b_id) = 0;
  virtual int32_t rmt_remove_edge_half(const int64_t node_a_id, const int64_t node_b_id) = 0;
  virtual void rmt_lock() = 0;
  virtual void rmt_unlock() = 0;
};

class RemoteServiceIfFactory {
 public:
  typedef RemoteServiceIf Handler;

  virtual ~RemoteServiceIfFactory() {}

  virtual RemoteServiceIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(RemoteServiceIf* /* handler */) = 0;
};

class RemoteServiceIfSingletonFactory : virtual public RemoteServiceIfFactory {
 public:
  RemoteServiceIfSingletonFactory(const boost::shared_ptr<RemoteServiceIf>& iface) : iface_(iface) {}
  virtual ~RemoteServiceIfSingletonFactory() {}

  virtual RemoteServiceIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(RemoteServiceIf* /* handler */) {}

 protected:
  boost::shared_ptr<RemoteServiceIf> iface_;
};

class RemoteServiceNull : virtual public RemoteServiceIf {
 public:
  virtual ~RemoteServiceNull() {}
  int32_t rmt_add_node(const int64_t /* node_id */) {
    int32_t _return = 0;
    return _return;
  }
  int32_t rmt_add_edge_half(const int64_t /* node_a_id */, const int64_t /* node_b_id */) {
    int32_t _return = 0;
    return _return;
  }
  int32_t rmt_remove_node(const int64_t /* node_id */) {
    int32_t _return = 0;
    return _return;
  }
  int32_t rmt_remove_edge(const int64_t /* node_a_id */, const int64_t /* node_b_id */) {
    int32_t _return = 0;
    return _return;
  }
  int32_t rmt_remove_edge_half(const int64_t /* node_a_id */, const int64_t /* node_b_id */) {
    int32_t _return = 0;
    return _return;
  }
  void rmt_lock() {
    return;
  }
  void rmt_unlock() {
    return;
  }
};

typedef struct _RemoteService_rmt_add_node_args__isset {
  _RemoteService_rmt_add_node_args__isset() : node_id(false) {}
  bool node_id :1;
} _RemoteService_rmt_add_node_args__isset;

class RemoteService_rmt_add_node_args {
 public:

  RemoteService_rmt_add_node_args(const RemoteService_rmt_add_node_args&);
  RemoteService_rmt_add_node_args& operator=(const RemoteService_rmt_add_node_args&);
  RemoteService_rmt_add_node_args() : node_id(0) {
  }

  virtual ~RemoteService_rmt_add_node_args() throw();
  int64_t node_id;

  _RemoteService_rmt_add_node_args__isset __isset;

  void __set_node_id(const int64_t val);

  bool operator == (const RemoteService_rmt_add_node_args & rhs) const
  {
    if (!(node_id == rhs.node_id))
      return false;
    return true;
  }
  bool operator != (const RemoteService_rmt_add_node_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RemoteService_rmt_add_node_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class RemoteService_rmt_add_node_pargs {
 public:


  virtual ~RemoteService_rmt_add_node_pargs() throw();
  const int64_t* node_id;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _RemoteService_rmt_add_node_result__isset {
  _RemoteService_rmt_add_node_result__isset() : success(false) {}
  bool success :1;
} _RemoteService_rmt_add_node_result__isset;

class RemoteService_rmt_add_node_result {
 public:

  RemoteService_rmt_add_node_result(const RemoteService_rmt_add_node_result&);
  RemoteService_rmt_add_node_result& operator=(const RemoteService_rmt_add_node_result&);
  RemoteService_rmt_add_node_result() : success(0) {
  }

  virtual ~RemoteService_rmt_add_node_result() throw();
  int32_t success;

  _RemoteService_rmt_add_node_result__isset __isset;

  void __set_success(const int32_t val);

  bool operator == (const RemoteService_rmt_add_node_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const RemoteService_rmt_add_node_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RemoteService_rmt_add_node_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _RemoteService_rmt_add_node_presult__isset {
  _RemoteService_rmt_add_node_presult__isset() : success(false) {}
  bool success :1;
} _RemoteService_rmt_add_node_presult__isset;

class RemoteService_rmt_add_node_presult {
 public:


  virtual ~RemoteService_rmt_add_node_presult() throw();
  int32_t* success;

  _RemoteService_rmt_add_node_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _RemoteService_rmt_add_edge_half_args__isset {
  _RemoteService_rmt_add_edge_half_args__isset() : node_a_id(false), node_b_id(false) {}
  bool node_a_id :1;
  bool node_b_id :1;
} _RemoteService_rmt_add_edge_half_args__isset;

class RemoteService_rmt_add_edge_half_args {
 public:

  RemoteService_rmt_add_edge_half_args(const RemoteService_rmt_add_edge_half_args&);
  RemoteService_rmt_add_edge_half_args& operator=(const RemoteService_rmt_add_edge_half_args&);
  RemoteService_rmt_add_edge_half_args() : node_a_id(0), node_b_id(0) {
  }

  virtual ~RemoteService_rmt_add_edge_half_args() throw();
  int64_t node_a_id;
  int64_t node_b_id;

  _RemoteService_rmt_add_edge_half_args__isset __isset;

  void __set_node_a_id(const int64_t val);

  void __set_node_b_id(const int64_t val);

  bool operator == (const RemoteService_rmt_add_edge_half_args & rhs) const
  {
    if (!(node_a_id == rhs.node_a_id))
      return false;
    if (!(node_b_id == rhs.node_b_id))
      return false;
    return true;
  }
  bool operator != (const RemoteService_rmt_add_edge_half_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RemoteService_rmt_add_edge_half_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class RemoteService_rmt_add_edge_half_pargs {
 public:


  virtual ~RemoteService_rmt_add_edge_half_pargs() throw();
  const int64_t* node_a_id;
  const int64_t* node_b_id;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _RemoteService_rmt_add_edge_half_result__isset {
  _RemoteService_rmt_add_edge_half_result__isset() : success(false) {}
  bool success :1;
} _RemoteService_rmt_add_edge_half_result__isset;

class RemoteService_rmt_add_edge_half_result {
 public:

  RemoteService_rmt_add_edge_half_result(const RemoteService_rmt_add_edge_half_result&);
  RemoteService_rmt_add_edge_half_result& operator=(const RemoteService_rmt_add_edge_half_result&);
  RemoteService_rmt_add_edge_half_result() : success(0) {
  }

  virtual ~RemoteService_rmt_add_edge_half_result() throw();
  int32_t success;

  _RemoteService_rmt_add_edge_half_result__isset __isset;

  void __set_success(const int32_t val);

  bool operator == (const RemoteService_rmt_add_edge_half_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const RemoteService_rmt_add_edge_half_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RemoteService_rmt_add_edge_half_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _RemoteService_rmt_add_edge_half_presult__isset {
  _RemoteService_rmt_add_edge_half_presult__isset() : success(false) {}
  bool success :1;
} _RemoteService_rmt_add_edge_half_presult__isset;

class RemoteService_rmt_add_edge_half_presult {
 public:


  virtual ~RemoteService_rmt_add_edge_half_presult() throw();
  int32_t* success;

  _RemoteService_rmt_add_edge_half_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _RemoteService_rmt_remove_node_args__isset {
  _RemoteService_rmt_remove_node_args__isset() : node_id(false) {}
  bool node_id :1;
} _RemoteService_rmt_remove_node_args__isset;

class RemoteService_rmt_remove_node_args {
 public:

  RemoteService_rmt_remove_node_args(const RemoteService_rmt_remove_node_args&);
  RemoteService_rmt_remove_node_args& operator=(const RemoteService_rmt_remove_node_args&);
  RemoteService_rmt_remove_node_args() : node_id(0) {
  }

  virtual ~RemoteService_rmt_remove_node_args() throw();
  int64_t node_id;

  _RemoteService_rmt_remove_node_args__isset __isset;

  void __set_node_id(const int64_t val);

  bool operator == (const RemoteService_rmt_remove_node_args & rhs) const
  {
    if (!(node_id == rhs.node_id))
      return false;
    return true;
  }
  bool operator != (const RemoteService_rmt_remove_node_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RemoteService_rmt_remove_node_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class RemoteService_rmt_remove_node_pargs {
 public:


  virtual ~RemoteService_rmt_remove_node_pargs() throw();
  const int64_t* node_id;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _RemoteService_rmt_remove_node_result__isset {
  _RemoteService_rmt_remove_node_result__isset() : success(false) {}
  bool success :1;
} _RemoteService_rmt_remove_node_result__isset;

class RemoteService_rmt_remove_node_result {
 public:

  RemoteService_rmt_remove_node_result(const RemoteService_rmt_remove_node_result&);
  RemoteService_rmt_remove_node_result& operator=(const RemoteService_rmt_remove_node_result&);
  RemoteService_rmt_remove_node_result() : success(0) {
  }

  virtual ~RemoteService_rmt_remove_node_result() throw();
  int32_t success;

  _RemoteService_rmt_remove_node_result__isset __isset;

  void __set_success(const int32_t val);

  bool operator == (const RemoteService_rmt_remove_node_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const RemoteService_rmt_remove_node_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RemoteService_rmt_remove_node_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _RemoteService_rmt_remove_node_presult__isset {
  _RemoteService_rmt_remove_node_presult__isset() : success(false) {}
  bool success :1;
} _RemoteService_rmt_remove_node_presult__isset;

class RemoteService_rmt_remove_node_presult {
 public:


  virtual ~RemoteService_rmt_remove_node_presult() throw();
  int32_t* success;

  _RemoteService_rmt_remove_node_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _RemoteService_rmt_remove_edge_args__isset {
  _RemoteService_rmt_remove_edge_args__isset() : node_a_id(false), node_b_id(false) {}
  bool node_a_id :1;
  bool node_b_id :1;
} _RemoteService_rmt_remove_edge_args__isset;

class RemoteService_rmt_remove_edge_args {
 public:

  RemoteService_rmt_remove_edge_args(const RemoteService_rmt_remove_edge_args&);
  RemoteService_rmt_remove_edge_args& operator=(const RemoteService_rmt_remove_edge_args&);
  RemoteService_rmt_remove_edge_args() : node_a_id(0), node_b_id(0) {
  }

  virtual ~RemoteService_rmt_remove_edge_args() throw();
  int64_t node_a_id;
  int64_t node_b_id;

  _RemoteService_rmt_remove_edge_args__isset __isset;

  void __set_node_a_id(const int64_t val);

  void __set_node_b_id(const int64_t val);

  bool operator == (const RemoteService_rmt_remove_edge_args & rhs) const
  {
    if (!(node_a_id == rhs.node_a_id))
      return false;
    if (!(node_b_id == rhs.node_b_id))
      return false;
    return true;
  }
  bool operator != (const RemoteService_rmt_remove_edge_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RemoteService_rmt_remove_edge_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class RemoteService_rmt_remove_edge_pargs {
 public:


  virtual ~RemoteService_rmt_remove_edge_pargs() throw();
  const int64_t* node_a_id;
  const int64_t* node_b_id;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _RemoteService_rmt_remove_edge_result__isset {
  _RemoteService_rmt_remove_edge_result__isset() : success(false) {}
  bool success :1;
} _RemoteService_rmt_remove_edge_result__isset;

class RemoteService_rmt_remove_edge_result {
 public:

  RemoteService_rmt_remove_edge_result(const RemoteService_rmt_remove_edge_result&);
  RemoteService_rmt_remove_edge_result& operator=(const RemoteService_rmt_remove_edge_result&);
  RemoteService_rmt_remove_edge_result() : success(0) {
  }

  virtual ~RemoteService_rmt_remove_edge_result() throw();
  int32_t success;

  _RemoteService_rmt_remove_edge_result__isset __isset;

  void __set_success(const int32_t val);

  bool operator == (const RemoteService_rmt_remove_edge_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const RemoteService_rmt_remove_edge_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RemoteService_rmt_remove_edge_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _RemoteService_rmt_remove_edge_presult__isset {
  _RemoteService_rmt_remove_edge_presult__isset() : success(false) {}
  bool success :1;
} _RemoteService_rmt_remove_edge_presult__isset;

class RemoteService_rmt_remove_edge_presult {
 public:


  virtual ~RemoteService_rmt_remove_edge_presult() throw();
  int32_t* success;

  _RemoteService_rmt_remove_edge_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _RemoteService_rmt_remove_edge_half_args__isset {
  _RemoteService_rmt_remove_edge_half_args__isset() : node_a_id(false), node_b_id(false) {}
  bool node_a_id :1;
  bool node_b_id :1;
} _RemoteService_rmt_remove_edge_half_args__isset;

class RemoteService_rmt_remove_edge_half_args {
 public:

  RemoteService_rmt_remove_edge_half_args(const RemoteService_rmt_remove_edge_half_args&);
  RemoteService_rmt_remove_edge_half_args& operator=(const RemoteService_rmt_remove_edge_half_args&);
  RemoteService_rmt_remove_edge_half_args() : node_a_id(0), node_b_id(0) {
  }

  virtual ~RemoteService_rmt_remove_edge_half_args() throw();
  int64_t node_a_id;
  int64_t node_b_id;

  _RemoteService_rmt_remove_edge_half_args__isset __isset;

  void __set_node_a_id(const int64_t val);

  void __set_node_b_id(const int64_t val);

  bool operator == (const RemoteService_rmt_remove_edge_half_args & rhs) const
  {
    if (!(node_a_id == rhs.node_a_id))
      return false;
    if (!(node_b_id == rhs.node_b_id))
      return false;
    return true;
  }
  bool operator != (const RemoteService_rmt_remove_edge_half_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RemoteService_rmt_remove_edge_half_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class RemoteService_rmt_remove_edge_half_pargs {
 public:


  virtual ~RemoteService_rmt_remove_edge_half_pargs() throw();
  const int64_t* node_a_id;
  const int64_t* node_b_id;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _RemoteService_rmt_remove_edge_half_result__isset {
  _RemoteService_rmt_remove_edge_half_result__isset() : success(false) {}
  bool success :1;
} _RemoteService_rmt_remove_edge_half_result__isset;

class RemoteService_rmt_remove_edge_half_result {
 public:

  RemoteService_rmt_remove_edge_half_result(const RemoteService_rmt_remove_edge_half_result&);
  RemoteService_rmt_remove_edge_half_result& operator=(const RemoteService_rmt_remove_edge_half_result&);
  RemoteService_rmt_remove_edge_half_result() : success(0) {
  }

  virtual ~RemoteService_rmt_remove_edge_half_result() throw();
  int32_t success;

  _RemoteService_rmt_remove_edge_half_result__isset __isset;

  void __set_success(const int32_t val);

  bool operator == (const RemoteService_rmt_remove_edge_half_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const RemoteService_rmt_remove_edge_half_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RemoteService_rmt_remove_edge_half_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _RemoteService_rmt_remove_edge_half_presult__isset {
  _RemoteService_rmt_remove_edge_half_presult__isset() : success(false) {}
  bool success :1;
} _RemoteService_rmt_remove_edge_half_presult__isset;

class RemoteService_rmt_remove_edge_half_presult {
 public:


  virtual ~RemoteService_rmt_remove_edge_half_presult() throw();
  int32_t* success;

  _RemoteService_rmt_remove_edge_half_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};


class RemoteService_rmt_lock_args {
 public:

  RemoteService_rmt_lock_args(const RemoteService_rmt_lock_args&);
  RemoteService_rmt_lock_args& operator=(const RemoteService_rmt_lock_args&);
  RemoteService_rmt_lock_args() {
  }

  virtual ~RemoteService_rmt_lock_args() throw();

  bool operator == (const RemoteService_rmt_lock_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const RemoteService_rmt_lock_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RemoteService_rmt_lock_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class RemoteService_rmt_lock_pargs {
 public:


  virtual ~RemoteService_rmt_lock_pargs() throw();

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class RemoteService_rmt_lock_result {
 public:

  RemoteService_rmt_lock_result(const RemoteService_rmt_lock_result&);
  RemoteService_rmt_lock_result& operator=(const RemoteService_rmt_lock_result&);
  RemoteService_rmt_lock_result() {
  }

  virtual ~RemoteService_rmt_lock_result() throw();

  bool operator == (const RemoteService_rmt_lock_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const RemoteService_rmt_lock_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RemoteService_rmt_lock_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class RemoteService_rmt_lock_presult {
 public:


  virtual ~RemoteService_rmt_lock_presult() throw();

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};


class RemoteService_rmt_unlock_args {
 public:

  RemoteService_rmt_unlock_args(const RemoteService_rmt_unlock_args&);
  RemoteService_rmt_unlock_args& operator=(const RemoteService_rmt_unlock_args&);
  RemoteService_rmt_unlock_args() {
  }

  virtual ~RemoteService_rmt_unlock_args() throw();

  bool operator == (const RemoteService_rmt_unlock_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const RemoteService_rmt_unlock_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RemoteService_rmt_unlock_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class RemoteService_rmt_unlock_pargs {
 public:


  virtual ~RemoteService_rmt_unlock_pargs() throw();

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class RemoteService_rmt_unlock_result {
 public:

  RemoteService_rmt_unlock_result(const RemoteService_rmt_unlock_result&);
  RemoteService_rmt_unlock_result& operator=(const RemoteService_rmt_unlock_result&);
  RemoteService_rmt_unlock_result() {
  }

  virtual ~RemoteService_rmt_unlock_result() throw();

  bool operator == (const RemoteService_rmt_unlock_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const RemoteService_rmt_unlock_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RemoteService_rmt_unlock_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class RemoteService_rmt_unlock_presult {
 public:


  virtual ~RemoteService_rmt_unlock_presult() throw();

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class RemoteServiceClient : virtual public RemoteServiceIf {
 public:
  RemoteServiceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  RemoteServiceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  int32_t rmt_add_node(const int64_t node_id);
  void send_rmt_add_node(const int64_t node_id);
  int32_t recv_rmt_add_node();
  int32_t rmt_add_edge_half(const int64_t node_a_id, const int64_t node_b_id);
  void send_rmt_add_edge_half(const int64_t node_a_id, const int64_t node_b_id);
  int32_t recv_rmt_add_edge_half();
  int32_t rmt_remove_node(const int64_t node_id);
  void send_rmt_remove_node(const int64_t node_id);
  int32_t recv_rmt_remove_node();
  int32_t rmt_remove_edge(const int64_t node_a_id, const int64_t node_b_id);
  void send_rmt_remove_edge(const int64_t node_a_id, const int64_t node_b_id);
  int32_t recv_rmt_remove_edge();
  int32_t rmt_remove_edge_half(const int64_t node_a_id, const int64_t node_b_id);
  void send_rmt_remove_edge_half(const int64_t node_a_id, const int64_t node_b_id);
  int32_t recv_rmt_remove_edge_half();
  void rmt_lock();
  void send_rmt_lock();
  void recv_rmt_lock();
  void rmt_unlock();
  void send_rmt_unlock();
  void recv_rmt_unlock();
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class RemoteServiceProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  boost::shared_ptr<RemoteServiceIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (RemoteServiceProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_rmt_add_node(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_rmt_add_edge_half(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_rmt_remove_node(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_rmt_remove_edge(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_rmt_remove_edge_half(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_rmt_lock(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_rmt_unlock(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  RemoteServiceProcessor(boost::shared_ptr<RemoteServiceIf> iface) :
    iface_(iface) {
    processMap_["rmt_add_node"] = &RemoteServiceProcessor::process_rmt_add_node;
    processMap_["rmt_add_edge_half"] = &RemoteServiceProcessor::process_rmt_add_edge_half;
    processMap_["rmt_remove_node"] = &RemoteServiceProcessor::process_rmt_remove_node;
    processMap_["rmt_remove_edge"] = &RemoteServiceProcessor::process_rmt_remove_edge;
    processMap_["rmt_remove_edge_half"] = &RemoteServiceProcessor::process_rmt_remove_edge_half;
    processMap_["rmt_lock"] = &RemoteServiceProcessor::process_rmt_lock;
    processMap_["rmt_unlock"] = &RemoteServiceProcessor::process_rmt_unlock;
  }

  virtual ~RemoteServiceProcessor() {}
};

class RemoteServiceProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  RemoteServiceProcessorFactory(const ::boost::shared_ptr< RemoteServiceIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< RemoteServiceIfFactory > handlerFactory_;
};

class RemoteServiceMultiface : virtual public RemoteServiceIf {
 public:
  RemoteServiceMultiface(std::vector<boost::shared_ptr<RemoteServiceIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~RemoteServiceMultiface() {}
 protected:
  std::vector<boost::shared_ptr<RemoteServiceIf> > ifaces_;
  RemoteServiceMultiface() {}
  void add(boost::shared_ptr<RemoteServiceIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  int32_t rmt_add_node(const int64_t node_id) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->rmt_add_node(node_id);
    }
    return ifaces_[i]->rmt_add_node(node_id);
  }

  int32_t rmt_add_edge_half(const int64_t node_a_id, const int64_t node_b_id) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->rmt_add_edge_half(node_a_id, node_b_id);
    }
    return ifaces_[i]->rmt_add_edge_half(node_a_id, node_b_id);
  }

  int32_t rmt_remove_node(const int64_t node_id) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->rmt_remove_node(node_id);
    }
    return ifaces_[i]->rmt_remove_node(node_id);
  }

  int32_t rmt_remove_edge(const int64_t node_a_id, const int64_t node_b_id) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->rmt_remove_edge(node_a_id, node_b_id);
    }
    return ifaces_[i]->rmt_remove_edge(node_a_id, node_b_id);
  }

  int32_t rmt_remove_edge_half(const int64_t node_a_id, const int64_t node_b_id) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->rmt_remove_edge_half(node_a_id, node_b_id);
    }
    return ifaces_[i]->rmt_remove_edge_half(node_a_id, node_b_id);
  }

  void rmt_lock() {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->rmt_lock();
    }
    ifaces_[i]->rmt_lock();
  }

  void rmt_unlock() {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->rmt_unlock();
    }
    ifaces_[i]->rmt_unlock();
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class RemoteServiceConcurrentClient : virtual public RemoteServiceIf {
 public:
  RemoteServiceConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  RemoteServiceConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  int32_t rmt_add_node(const int64_t node_id);
  int32_t send_rmt_add_node(const int64_t node_id);
  int32_t recv_rmt_add_node(const int32_t seqid);
  int32_t rmt_add_edge_half(const int64_t node_a_id, const int64_t node_b_id);
  int32_t send_rmt_add_edge_half(const int64_t node_a_id, const int64_t node_b_id);
  int32_t recv_rmt_add_edge_half(const int32_t seqid);
  int32_t rmt_remove_node(const int64_t node_id);
  int32_t send_rmt_remove_node(const int64_t node_id);
  int32_t recv_rmt_remove_node(const int32_t seqid);
  int32_t rmt_remove_edge(const int64_t node_a_id, const int64_t node_b_id);
  int32_t send_rmt_remove_edge(const int64_t node_a_id, const int64_t node_b_id);
  int32_t recv_rmt_remove_edge(const int32_t seqid);
  int32_t rmt_remove_edge_half(const int64_t node_a_id, const int64_t node_b_id);
  int32_t send_rmt_remove_edge_half(const int64_t node_a_id, const int64_t node_b_id);
  int32_t recv_rmt_remove_edge_half(const int32_t seqid);
  void rmt_lock();
  int32_t send_rmt_lock();
  void recv_rmt_lock(const int32_t seqid);
  void rmt_unlock();
  int32_t send_rmt_unlock();
  void recv_rmt_unlock(const int32_t seqid);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
  ::apache::thrift::async::TConcurrentClientSyncInfo sync_;
};

#ifdef _WIN32
  #pragma warning( pop )
#endif

} // namespace

#endif
