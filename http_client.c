#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 4096

void print_usage() {
    printf("Usage: ./http_client [-options] server_url port_number\n");
    printf("Options:\n");
    printf("  -p : Print RTT for the request\n");
}

void calculate_rtt(struct timespec start, struct timespec end) {
    long seconds = end.tv_sec - start.tv_sec;
    long nanoseconds = end.tv_nsec - start.tv_nsec;
    double milliseconds = seconds * 1000.0 + nanoseconds / 1000000.0;
    printf("RTT: %.2f ms\n", milliseconds);
}

int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 4) {
        print_usage();
        return 1;
    }

    int print_rtt = 0;
    char *server_url;
    int port;

    if (argc == 4 && strcmp(argv[1], "-p") == 0) {
        print_rtt = 1;
        server_url = argv[2];
        port = atoi(argv[3]);
    } else if (argc == 3) {
        server_url = argv[1];
        port = atoi(argv[2]);
    } else {
        print_usage();
        return 1;
    }

    // Debug: Print parsed command-line arguments
    printf("Server URL: %s\n", server_url);
    printf("Port: %d\n", port);
    if (print_rtt) {
        printf("RTT measurement enabled\n");
    }

    // Extract hostname and path from the URL
    char hostname[256] = {0};
    char path[256] = "/";

    if (strstr(server_url, "http://") == server_url) {
        sscanf(server_url, "http://%255[^/]/%255[^\n]", hostname, path);
    } else if (strstr(server_url, "https://") == server_url) {
        sscanf(server_url, "https://%255[^/]/%255[^\n]", hostname, path);
    } else {
        sscanf(server_url, "%255[^/]/%255[^\n]", hostname, path);
    }

    // If the server URL doesn't contain a path, default to "/"
    if (strlen(path) == 0) {
        strcpy(path, "/");
    }
    // Debug: Print extracted hostname and path
    printf("Hostname: %s\n", hostname);
    printf("Path: %s\n", path);

    

    // Create a TCP socket
    int client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock < 0) {
        perror("Socket creation failed");
        return 1;
    }
    printf("Socket created successfully\n");

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    // Resolve hostname to IP
    struct hostent *host = gethostbyname(hostname);
    if (host == NULL) {
        fprintf(stderr, "Error: Could not resolve hostname %s\n", hostname);
        close(client_sock);
        return 1;
    }

    // Copy the first IP address from h_addr_list to server_addr.sin_addr
    memcpy(&server_addr.sin_addr, host->h_addr_list[0], sizeof(struct in_addr));

    // Debug: Print the resolved IP address
    printf("Resolved IP address: %s\n", inet_ntoa(*(struct in_addr *)host->h_addr_list[0]));


    // Measure RTT
    struct timespec start, end;
    if (print_rtt) {
        clock_gettime(CLOCK_MONOTONIC, &start);
    }


    // Connect to the server
    if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(client_sock);
        return 1;
    }
    printf("Connected to the server\n");

    if (print_rtt) {
        clock_gettime(CLOCK_MONOTONIC, &end);
        calculate_rtt(start, end);
    }

    // Create the HTTP GET request
    char request[BUFFER_SIZE];
    snprintf(request, sizeof(request),
             "GET /%s HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Connection: close\r\n\r\n",
             path, hostname);

    // Debug: Print the HTTP request
    printf("HTTP Request:\n%s\n", request);

    // Send the request
    if (send(client_sock, request, strlen(request), 0) < 0) {
        perror("Failed to send request");
        close(client_sock);
        return 1;
    }
    printf("Request sent successfully\n");

    // Receive and display the response
    char response[BUFFER_SIZE];
    int bytes_received;
    printf("Response:\n");
    while ((bytes_received = recv(client_sock, response, sizeof(response) - 1, 0)) > 0) {
        response[bytes_received] = '\0';
        printf("%s", response);
    }

    if (bytes_received < 0) {
        perror("Failed to receive response");
    } else {
        printf("\nResponse received successfully\n");
    }

    close(client_sock);
    return 0;
}
