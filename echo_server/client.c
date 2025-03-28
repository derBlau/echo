#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>


void displayMenu(void);
int sendMessage(int *client);
void printMessage(int *client);


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

  while (1) {
    displayMenu();
    int rsp = sendMessage(&client);
    if (rsp == 1) {
      break;
    }
    if (rsp == 0) {
      printMessage(&client);
    }
   }
  
  close(client);
  printf("The connection to the server was closed.\n");
  return 0;
}


void displayMenu(void)
{
  printf("Write your message and press enter to send.\n");
  printf("Enter !q to finish the connection\n");
}

int sendMessage(int *client)
{
  size_t bufferSize = 1024;
  char buffer[bufferSize];

  printf(">> ");
  fgets(buffer, bufferSize, stdin);
  size_t len = strcspn(buffer, "\n");
  buffer[len] = '\0';
  
  if (len == 0) {
    printf("You cannot send an empty message\n");
    return -1;
    }
  if (strcmp("!q", buffer) == 0) {
    printf("The connection to the client will be closed\n");
    return 1;
  }
  
  send(*client, buffer, len + 1, 0);
  return 0;
}

void printMessage(int *client)
{
  size_t bufferSize = 1024;
  char buffer[bufferSize];
  ssize_t data = recv(*client, buffer, bufferSize, 0);
  if (data == 0) {
    printf("Connection to the server was lost.\n");
  }
  if (data > 0) {
    buffer[data] = '\0';
    printf("server response >> %s\n", buffer);
  }
}
