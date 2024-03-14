client:
	mkdir -p build/
	cc client.c -o build/client
server:
	mkdir -p build/
	cc server.c -o buildserver
all:
	mkdir -p build/
	cc client.c -o build/client
	cc server.c -o build/server
clean:
	rm -rf build/server
	rm -rf build/client

