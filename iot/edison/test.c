/*
 * Version: 0.1
 * Platform: Linux
 * Author: Aiwei Yu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char **argv)
{
    int listenfd;
    int connfd;
    struct sockaddr_in servaddr;

    listenfd = socket(PF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(50001);

    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    while(1)
    {
        listen(listenfd, 10);
        connfd = accept(listenfd, (struct sockaddr *)NULL, NULL);

        int n;
        char recvline[1024];

        while((n=read(connfd, recvline, 1024)) > 0)
        {

            char bufx[20];
            char bufy[20];
            recvline[n] = 0;
            printf("%s\n", recvline);

            /*string split*/

            int j = 0;

            //split x
            while(recvline[j] != 'x')
            {
                bufx[j] = recvline[j];
                j++;
            }

            //convert to double
            double x = atof(bufx);
            while(recvline[j] == 'x')
                j++;
            int l = 0;
            while(recvline[j] != 'x')
            {
                bufy[l++] = recvline[j++];
            }
            double y = atof(bufy);
            printf("%f\n",x);
            printf("%f\n",y);
        }
    }
    close(connfd);
    close(listenfd);

    return 1;
}
