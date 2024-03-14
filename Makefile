client:
	cc client.c -o client
server:
	cc server.c -o server
all:
	cc client.c -o client
	cc server.c -o server
clean:
	rm -rf server
	rm -rf client

