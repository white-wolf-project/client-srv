CC = gcc 
SRC = src
DEBUG ?=
arch = $(shell arch)
ifeq ($(DEBUG), 1)
	DBG = -DDEBUG
endif

OBJECTS = src/client.o \
		  src/client_tool.o\
		  src/server.o \
		  src/server_tool.o \
		  src/xml.o

all : client server

client :  src/client.o src/client_tool.o src/xml.o
	@echo "LD      $@"
	@$(CC) src/client.o src/client_tool.o src/xml.o $(DBG) -lxml2 -o  $@

server : src/server.o src/server_tool.o src/xml.o
	@echo "LD      $@"
	@$(CC) src/server.o src/server_tool.o src/xml.o  $(DBG) -lxml2 -o  $@

$(SRC)/%.o : $(SRC)/%.c
	@echo "CC	$<"
	@$(CC) -c -g $(DBG) -I. -I/usr/include/libxml2 $< -o $@

clean :
	rm -rf client server src/*.o \
	deb *.deb

deb : client server
	@echo "Packing..."
	mkdir -p deb/usr/local/bin/ deb/DEBIAN
	cp client server deb/usr/local/bin
	cp resources/control deb/DEBIAN
	dpkg-deb --build deb client-srv_0.1_$(arch).deb
	@echo "done"
