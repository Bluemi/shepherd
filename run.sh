#!/bin/bash

if [ "$1" == "s" ]; then
	./build/bin/server
elif [ "$1" == "t" ]; then
	./build/tests/bin/packet_helper_test
else
	./build/bin/client "localhost" "player0"
fi
