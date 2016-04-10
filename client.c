#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

int main(void)
{
     int fd = 0;
     int conn_fd = 0;
     int socklen = 0;
     int retVal = 0;
     char buf[256] = {0};
     char str[128] = {0};
     struct sockaddr_in addr_c;

     memset(&addr_c, 0, sizeof(addr_c));
     addr_c.sin_family = AF_INET;
     addr_c.sin_port = htons(4096);
     addr_c.sin_addr.s_addr = inet_addr("127.0.0.1");

     fd = socket(AF_INET, SOCK_STREAM, 0);
     if ( -1 == fd)_exit(-1);

     retVal = connect(fd, (struct sockaddr*)&addr_c, sizeof(addr_c));
     if ( -1 == retVal ) _exit(-1);

     while(1)
     {

          memset(str, 0, 128);
          gets(str);

          retVal = send(fd, str, strlen(str), 0);
          if( -1 == retVal) _exit(-1);

          if(strcmp(str, "exit")==0)
          {
                close(fd);
                _exit(0);
          }

     }

     return 0;
}

