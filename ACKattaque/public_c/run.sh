#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 port_number"
    exit 1
fi

# Store the port number
PORT_NUMBER="$1"

# Run the bot
chmod +x my_bot
./my_bot $PORT_NUMBER
