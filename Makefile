INC = ../inc
SRC = ../src
BIN = ../bin
OBJ = ../obj

CFLAGS = -c -g -Wall
LFLAGS = -o
G++ = g++ -fprofile-arcs -ftest-coverage
IFLAGS = -I $(INC)



all : $(BIN)/client $(BIN)/server

$(OBJ)/hello.o : $(SRC)/hello.cpp
	$(G++) $(CFLAGS) $(SRC)/hello.cpp $(IFLAGS)
	mv *.o ../obj

$(OBJ)/client.o : $(SRC)/client.cpp
	$(G++) $(CFLAGS) $(SRC)/client.cpp $(IFLAGS)
	mv *.o ../obj

$(OBJ)/server.o : $(SRC)/server.cpp
	$(G++) $(CFLAGS) $(SRC)/server.cpp $(IFLAGS)
	mv *.o ../obj

$(BIN)/client : $(OBJ)/login.o $(OBJ)/client.o
	$(G++) $(OBJ)/login.o $(OBJ)/client.o $(LFLAGS) $(BIN)/client

$(BIN)/server : $(OBJ)/login.o $(OBJ)/server.o
	$(G++) $(OBJ)/login.o $(OBJ)/server.o $(LFLAGS) $(BIN)/server

clean:
	rm -f $(OBJ)/ *.o $(BIN)/client $(BIN)/server
                                                
