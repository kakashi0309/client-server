#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <pthread.h>

void * handleConnection( void *fd )
{
    int ret = 0;
    int socket_fd = *(int*)fd;
    char *buf = (char*)malloc(256);
    if (buf != NULL)
    {
          printf("new client connected...... socket = %d\n", socket_fd);
          while(1)
         {
         memset(buf, 0, 256);
         ret = recv(socket_fd, buf, sizeof(buf), 0);
         if(ret > 0)
          {printf("%s\n", buf);
          if(strcmp(buf, "exit") == 0)
                {close(socket_fd);
                 free(buf);
               pthread_exit(0);
                }
          }
         }

    }
    

}

int main(void)
{
     int fd = 0;
     int conn_fd = 0;
     int flags = 0;
     int isConnOK = 0;
     int socklen = 0;
     int retVal = 0;
     fd_set rfds;
     pthread_t pid;
     char buf[256] = {0};
     char str[128] = {0};
     struct sockaddr_in addr_s;
     struct sockaddr_in addr_c;
     struct timeval timeout;
     timeout.tv_sec = 5;
     timeout.tv_usec = 0;
     FD_ZERO(&rfds);
     FD_SET(fd, &rfds);
     memset(&addr_s, 0, sizeof(addr_s));
     memset(&addr_c, 0, sizeof(addr_c));
     addr_s.sin_family = AF_INET;
     addr_s.sin_port = htons(4096);
     addr_s.sin_addr.s_addr = INADDR_ANY;

     fd = socket(AF_INET, SOCK_STREAM, 0);
     if ( -1 == fd)_exit(-1);

     //flags = fcntl(fd, F_GETFL, 0);
     //retVal = fcntl(fd, F_SETFL, flags |  O_NONBLOCK);
     //if( retVal != 0 ) _exit(-1);

     retVal = bind(fd, (struct sockaddr*)&addr_s, sizeof(addr_s));
     if(-1 == retVal) _exit(-1);

     listen(fd, 15);

     socklen = sizeof(addr_c);
     //conn_fd = accept(fd, (struct sockaddr*)&addr_c, &socklen);
     //if ( -1 == conn_fd) _exit(-1);

     while(1)
     {
          FD_ZERO(&rfds);
          FD_SET(fd, &rfds);
          timeout.tv_sec = 5;
          timeout.tv_usec = 0;
          printf("start to select ......\n");
          retVal = select(fd+1, &rfds, NULL, NULL, &timeout);
          printf("retVal = %d\n", retVal);
          if( -1 == retVal || 0 == retVal) continue;
          if(FD_ISSET(fd, &rfds))
          {
                printf("There is read fds ...\n");
                if(isConnOK<5){
                conn_fd = accept(fd, (struct sockaddr*)&addr_c, &socklen);
                if ( -1 == conn_fd ) printf("Failed to accept client...\n");
		pthread_create(&pid, NULL, (void*)handleConnection, (void*)&conn_fd);
                isConnOK += 1;
		continue;
                }
         
	}
     }


     return 0;
}

