#!/bin/bash

ssh charles@104.42.186.101 'export LD_LIBRARY_PATH="/usr/local/lib" ; /home/charles/yyzhang/MyDistributedSystem/cs426_graph_server -a "6666 2333" > log &'
ssh yyc@40.117.95.162 'export LD_LIBRARY_PATH="/usr/local/lib" ; /home/yyc/yyzhang/MyDistributedSystem/cs426_graph_server -a "6666 2333" -b "104.42.186.101 6666 2333" > log &'
ssh yyzhang@168.62.177.50 'export LD_LIBRARY_PATH="/usr/local/lib" ; /home/yyzhang/cs426/ass3/MyDistributedSystem/cs426_graph_server -a "6666 2333" -b "40.117.95.162 6666 2333" > log &'
