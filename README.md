# PortChat

[![Build Status](https://travis-ci.com/FreddieBrown/PortChat.svg?branch=develop)](https://travis-ci.com/FreddieBrown/PortChat)

This is PortChat, a way to communicate with another user on the same network using your terminal.
This works using socket programming in C where it opens a port and accepts input accross that.

## Features

This program has a number of different ways to interact with it and use it.

### Server

This is the option which turns the machine into the central node of the conversation. This opens
the port on the users machine and accepts connections from clients (other computers).

To do this, do: `./main -s <port_number>` or `./main --sserver <port_number>` where port_number is
the name of the port you want to open up to accept connections to.

Once a connection is up, the user can send messages from the server to the client.

### Client

This feature is used to connect to a server node. This is so messages can be sent to the server.
To use this feature, a server node has to be running. Once it is, get the IP address from the server
(it's printed to the terminal) and use the command `./main -c <ip_address> <port_number>` or `./main --client <ip_address> <port_number>`. This will connect the client to the server. 

From the point of connection, messages can be sent and received to/from the server.

### Logging

This option is used to record everything that is sent and received during a connection. Everything is written to a file called `messages.txt`. In this file, a user can access the history of a conversation that has been had.

To use this, add the flag `-l` along with using the client or server command to turn on logging, e.g `./main -l -s 8080` or `./main -s 8080 -l`.

### Help

Option is here to explain to a user how to use the program. To use this, type `./main -h` or `./main --help`. This can also be used in conjunction with other commands to show the help screen as well as other options.

## Build Instructions

To build the program, use the make commands defined in the Makefile.

To clean the objects directory and delete the binary, use `make clean`.

To build the project, use `make build`.

You can also use `make run port=<port_number>` to run the project after it has been built.

## How to get involved

If you want to get involved in this project there are a few ways to do this.

The first option is to just clone the repo, make a change and then submit a PR. This will then be reviewed by someone before it can go into develop.

Another option is to pick up an task on the project board or try and solve an issue. Like above, clone the code, make your change and then submit a PR but make sure you reference the issue or task in your PR message so it can be dealt with and closed/moved on the board.
