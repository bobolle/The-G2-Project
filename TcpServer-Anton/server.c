#include <arpa/inet.h>
#include <errno.h>
#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#define DEFAULT_PORT 1337
#define BUFFER_SIZE 1024

int server_fd = -1;
int client_socket = -1;

void handle_signint(int sig) {
  printf("\nClosing server..\n");
  if (client_socket != -1) {
    close(client_socket);
    printf("Client socket closed.\n");
  }
  if (server_fd != -1) {
    close(server_fd);
    printf("Server socket closed.\n");
  }
  exit(0);
}

int main(int argc, char *argv[]) {
  signal(SIGINT, handle_signint);

  // Read flags for port or use standard
  int opt;
  int port = DEFAULT_PORT;

  while ((opt = getopt(argc, argv, "p:")) != -1) {
    switch (opt) {
      case 'p':
        if (optarg) {
          port = atoi(optarg);
          if (port <= 0) {
            fprintf(stderr, "Invalid port number.\n");
            return EXIT_FAILURE;
          }
        }
        break;
      default:
        fprintf(stderr, "Usage: %s [-p port].\n", argv[0]);
        return EXIT_FAILURE;
    }
  }

  struct sockaddr_in address = {
      .sin_family = AF_INET,  // IPv4
      .sin_addr.s_addr = INADDR_ANY,
      .sin_port = htons(port)  // Convert to network byte order
  };

  // Create socket file descriptor
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1) {
    perror("Socket creation failed!\n");
    exit(EXIT_FAILURE);
  }

  opt = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  // Bind the socket to the specified port
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("Bind failed!\n");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // Start listening for incoming connections
  if (listen(server_fd, 3) < 0) {
    perror("Listen failed!\n");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  printf("Server is listening on port %d\n", port);

  char buffer[BUFFER_SIZE] = {0};

  while (1) {
    // Accept incoming connection
    socklen_t addr_len = sizeof(address);
    printf("Waiting for incoming connections...\n");

    client_socket =
        accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addr_len);
    if (client_socket < 0) {
      perror("Accept failed!");
      continue;
    }

    printf("Client connected from %s:%d\n", inet_ntoa(address.sin_addr),
           ntohs(address.sin_port));

    // Handle client timeout
    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout,
               sizeof(timeout));

    // Initialized buffer
    char buffer[BUFFER_SIZE] = {0};

    while (1) {
      // Read data from client
      int bytes_recieved = recv(client_socket, buffer, BUFFER_SIZE, 0);
      if (bytes_recieved < 0) {
        if (errno = EAGAIN || errno == EWOULDBLOCK) {
          printf(
              "Timeout: Client has been inactive too long. Closing "
              "connection.\n");
        } else {
          perror("Error while receiving data.\n");
        }
        close(client_socket);
        client_socket = -1;
        break;
      } else if (bytes_recieved == 0) {
        printf("Client disconnected.\n");
        close(client_socket);
        client_socket = -1;
        break;
      }

      buffer[bytes_recieved] = '\0';
      printf("Received from client: %s\n", buffer);

      // Send response
      const char *response = "Message received!\n";
      if (send(client_socket, response, strlen(response), 0) < 0) {
        perror("Error while sending reply.\n");
      }
    }
  }

  return 0;
}
