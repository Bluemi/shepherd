#!/bin/bash

if [ "$1" == "s" ]; then
	./build/bin/server
else
	./build/bin/client
fi
