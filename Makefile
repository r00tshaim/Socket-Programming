all: server client

server: server.cpp
	@echo "compiling server"
	g++ server.cpp -o server

client: client.cpp
	@echo "compiling client"
	g++ client.cpp -o client

clean:
	rm -f server client

