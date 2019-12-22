#!/bin/bash

if [ "$1" == "s" ]; then
	./build/bin/server
elif [ "$1" == "t" ]; then
	./build/tests/bin/packet_helper_test
else
	./build/bin/client player0
fi
