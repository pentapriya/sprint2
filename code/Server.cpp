#include<iostream>
#include <stdio.h>
#include <signal.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include<unistd.h>
using namespace std;
#define MAX 80
#define PORT 7070
#define SA struct sockaddr


int connfd;

void func(int connfd)
{
    char buff[MAX];
    int n;
    // infinite loop for chat
    for (;;) {
        bzero(buff, MAX);

        // read the message from client and copy it in buffer
        read(connfd, buff, sizeof(buff));
        // print buffer which contains the client contents
        cout<<"From Admin: "<<buff<<endl;
	if (strncmp("exit", buff, 4) == 0) {
            cout<<"Server Exit..."<<endl;
            break;
        }
	cout<<"to User: ";
        bzero(buff, MAX);
        n = 0;
        // copy server message in the buffer
        while ((buff[n++] = getchar()) != '\n')
            ;

        // and send that buffer to client
        write(connfd, buff, sizeof(buff));

        // if msg contains "Exit" then server exit and chat ended.
        if (strncmp("exit", buff, 4) == 0) {
            cout<<"Server Exit..."<<endl;
            break;
        }
    }
}
void sigtstp_handler(int signal)
{
	char msg[MAX];
	strcpy(msg, "exit");
	cout << "\nCaught ^Z "<< endl;
	cout << "Closing server connection " <<endl;
	write(connfd, msg, sizeof(msg));	
	exit(0);
}


// Driver function
int main()
{
    int sockfd;
    struct sockaddr_in servaddr, cli;
    signal(SIGTSTP, sigtstp_handler);

    socklen_t clilen;
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        cout<<"socket creation failed..."<<endl;
        exit(0);
    }
    else
        cout<<"Socket successfully created.."<<endl;
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        cerr<<"socket bind failed..."<<endl;
        exit(0);
    }
    else
        cout<<"Socket successfully binded.."<<endl;

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        cerr<<"Listen failed..."<<endl;
        exit(0);
    }
    else
        cout<<"Server listening.."<<endl;
    //len = sizeof(cli);
   clilen = sizeof(cli);
    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA*)&cli, &clilen);
    if (connfd < 0) {
        cerr<<"server accept failed..."<<endl;
        exit(0);
    }
    else
        cout<<"server accept the client..."<<endl;

    // Function for chatting between client and server
    func(connfd);

    // After chatting close the socket
    close(sockfd);
}
