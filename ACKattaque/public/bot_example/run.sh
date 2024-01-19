#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 port_number"
    exit 1
fi

# Store the port number
PORT_NUMBER="$1"

export PYTHONPATH="/:$PYTHONPATH"

python my_bot.py "$PORT_NUMBER"
