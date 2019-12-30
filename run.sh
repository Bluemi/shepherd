#!/bin/bash

mode='debug'

if [ "$1" == "s" ]; then
	./build/${mode}/bin/server
elif [ "$1" == "t" ]; then
	./build/${mode}/tests/bin/packet_helper_test
elif [ "$1" == "r" ]; then
	./build/${mode}/bin/client "192.168.178.89" "player0"
else
	./build/${mode}/bin/client "localhost" "player0"
fi
