#!/bin/bash

mode='debug'
mode='release'

if [ "$1" == "s" ]; then
	LD_LIBRARY_PATH="$PWD/netsi/build/release/lib" ./build/${mode}/bin/server
elif [ "$1" == "t" ]; then
	./build/${mode}/tests/bin/packet_helper_test
elif [ "$1" == "r" ]; then
	./build/${mode}/bin/client "192.168.178.89" "alok"
else
	LD_LIBRARY_PATH="$PWD/netsi/build/release/lib" ./build/${mode}/bin/client "localhost" "player0"
fi
