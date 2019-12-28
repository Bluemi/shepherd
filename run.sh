#!/bin/bash

if [ "$1" == "s" ]; then
	./build/bin/server
elif [ "$1" == "t" ]; then
	./build/tests/bin/packet_helper_test
elif [ "$1" == "r" ]; then
	./build/bin/client "192.168.178.89" "player0"
else
	./build/bin/client "localhost" "player0"
fi
