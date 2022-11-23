CC=g++
CFLAGS=-c -Wall

all: lms

lms: main.cpp
	$(CC) main.cpp -o lms -l sqlite3

Server: Server.cpp
	$(CC) Server.cpp -o Server

Client: Client.cpp
	$(CC) Client.cpp -o Client
