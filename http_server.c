#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

#define PORT 5000 // Default port for the server

void handle_TCP_Client(int client_socket) {
    //Make buffer size large enough for requests
    char buffer[4096];
    int bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read < 0) {
        perror("Failed to read bytes from client");
        close(client_socket);
        return;
    }
    
    buffer[bytes_read] = '\0'; // Null-terminate the request so system reads properly
    printf("Client request:\n%s\n", buffer);

    char method[16], path[256];
    sscanf(buffer, "%s %s", method, path);

    if (path[0] == '/')
        memmove(path, path + 1, strlen(path));

    // If path is empty, just default to the index file
    if (strlen(path) == 0)
        strcpy(path, "index.html");

    // Respond by sending the content of HTML
    FILE *file = fopen(path, "r");
    if (file) {
        char response[4096] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
        char file_content[4096];
        size_t content_length = fread(file_content, 1, sizeof(file_content) - strlen(response) - 1, file);
        fclose(file);

        file_content[content_length] = '\0'; // Null-terminate the content
        strcat(response, file_content);
        send(client_socket, response, strlen(response), 0);
    } else {
        const char *response = "HTTP/1.1 404 Not Found\r\nContent-Length: 13\r\n\r\n404 Not Found";
        send(client_socket, response, strlen(response), 0);
    }

    close(client_socket);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Correct command: ./http_server port_number\n");
        return 1;
    }

    int port = atoi(argv[1]);

    // Create a TCP socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Scoket binding failed");
        return 1;
    }

    //Begin listening on port / socket
    if (listen(server_socket, 5) < 0) {
        perror("Listen failed");
        return 1;
    }

    printf("Server listening on port %d...\n", port);

    while (1) {
        // Accept a connection from a client
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket < 0) {
            perror("Connection accept failed");
            return 1;
        }

        // Handle the client connection
        handle_TCP_Client(client_socket);
    }

    // Close the server socket (unreachable code due to infinite loop)
    close(server_socket);
    return 0;
}
