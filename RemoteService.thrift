#struct Work {
#  1: i32 num1 = 0,
#  2: i32 num2,
#  3: Operation op,
#  4: optional string comment,
#}

#enum Operation {
#  ADD = 1,
#  SUBTRACT = 2,
#  MULTIPLY = 3,
#  DIVIDE = 4
#}

namespace cpp remote

service RemoteService {

  /**
   * A method definition looks like C code. It has a return type, arguments,
   * and optionally a list of exceptions that it may throw. Note that argument
   * lists and exception lists are specified using the exact same syntax as
   * field lists in struct or exception definitions.
   */

   #void graph_init(),

   i32 rmt_add_node(1:i64 node_id),

   i32 rmt_add_edge(1:i64 node_a_id, 2:i64 node_b_id),

   i32 rmt_remove_node(1:i64 node_id),

   i32 rmt_remove_edge(1:i64 node_a_id, 2:i64 node_b_id),

}