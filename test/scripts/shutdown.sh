#!/bin/bash

SHUTDOWN="ps aux | grep 'cs426_graph_server -a' | head -1 | awk '{print \$2}' | xargs kill"

ssh charles@104.42.186.101 $SHUTDOWN
ssh yyc@40.117.95.162 $SHUTDOWN
ssh yyzhang@168.62.177.50 $SHUTDOWN
