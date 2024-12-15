
all: http_client http_server


echoserver: echoserver.c
	gcc -o http_server http_server.c -I.

http_client: http_client.c
	gcc -o http_client http_client.c -I.


clean:
	rm -f http_server http_client