#include <stdio.h>
#include <stdlib.h>
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

  int client = socket(AF_INET, SOCK_STREAM, 0);
  if (client < 0) {
    perror("The client socket could not be created\n");
    exit(-1);
  }
  int connectStatus = connect(client, address, serverLen);
  if (connectStatus != 0) {
    perror("The server could not be reached.\n");
    close(client);
    return(-1);
  }
  printf("A connection to the server was established on port %i.\n", port);

  
  close(client);
  printf("The connection to the server was closed.\n");
  return 0;
}
