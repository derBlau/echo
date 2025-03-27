#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>


int main(void)
{
  
  int port = 8000;
  
  struct in_addr ip;
  ip.s_addr = htonl(INADDR_ANY);

  struct sockaddr_in serverInfo;
  serverInfo.sin_family = AF_INET;
  serverInfo.sin_port = htons(port);
  serverInfo.sin_addr = ip;
  socklen_t serverLen = sizeof(serverInfo);

  struct sockaddr *address = (struct sockaddr *)&serverInfo;

  struct sockaddr_in clientInfo;
  struct sockaddr *clientAddr = (struct sockaddr *)&clientInfo;
  socklen_t clientLen = sizeof(clientInfo);

  int server = socket(AF_INET, SOCK_STREAM, 0);
  if (server < 0) {
    perror("The server could not be initialised.\n");
    return -1;
  }
  int binding = bind(server, address, serverLen);
  if (binding != 0) {
    perror("The socket could not be bound to the address.\n");
    close(server);
    return 1;
  }
  int listening = listen(server, 0);
  if (listening != 0) {
    perror("The server could not start listening for connections.\n");
    close(server);
    return 1;
  }
  int connection = accept(server, clientAddr, &clientLen);
  if (connection < 0) {
    perror("The client could not be reached.\n");
    close(server);
    return 1;
  }
  printf("A connection was established\n");

  while (1) {
    size_t bufferSize = 1024;
    char buffer[bufferSize];

    ssize_t data = recv(connection, buffer, bufferSize -1, 0);
    if (data < 0) {
      printf("There was an error. The connection to the client will be terminated.\n");
      break;
    }
    if (data == 0) {
      printf("The connection to the client has been lost.\n");
      break;
    }
    buffer[bufferSize] = '\0';
    printf(">> %s\n", buffer);
  }

  close(connection);
  close(server);

  printf("The server has shut down.\n");
  return 0;
}
