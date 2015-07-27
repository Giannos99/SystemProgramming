#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include"head.h"

#define PORT "8888"   // port we're listening on

#define handle_error_en(en, msg) \
        do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

pthread_mutex_t mtx;
pthread_cond_t cond;

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

//Worker Thread
void * threadfunction(void * ptr1)
{
	//char * buffer;
	//int len;
	
	//connection_t * conn;
	//long addr = 0;
	struct client_message msG;
	int i=0;
	//struct lista *ptr;
	//ptr=head;
	
	/*if (!ptr) pthread_exit(0); 
	conn = (connection_t *)ptr;*/

	/* read length of message */
	printf("Mphke sto thread\n");
	pthread_mutex_lock(&mtx);
	pthread_cond_wait(&cond, &mtx);
	pthread_mutex_unlock(&mtx);
	  while(1){
	  	
	  	for(i=0;i<MAX_DATA_FOR_BUF;i++){
	  		msG.message[i]='\0';
	  	}
	  	msG.fd=-1;
	  	msG.len=-1;
		
		printf("Mphke sto thread katw ap to condition variable\n");
		if(head==NULL){
			printf("Mphke se ena thread xwris na exei h lista prammata mesa\n");
			
			break;
		}
		else{
			pthread_mutex_lock(&mtx);
			msG=head->client_message;
			pop_lista();
			pthread_mutex_unlock(&mtx);
			//len=memcpy(&len,(msG.message),sizeof(int));
			printf("EImai sto thread %d message = %s len=%d\n",pthread_self(),msG.message,msG.len);
			
			printf("W8\n");
			
			
			
		}
		printf("eimaste meta thn else sto pthread panw apo to mutex lock\n");
		pthread_mutex_lock(&mtx);
		printf("eimaste meta thn else sto pthread katw apo to mutex lock panw apo to cond wait\n");
		pthread_cond_wait(&cond, &mtx);
		printf("eimaste meta thn else sto pthread katw apo to cond wait panw ap to mutex unlock\n");
		pthread_mutex_unlock(&mtx);
		printf("eimaste meta thn else sto pthread katw apo to mutex unlock \n");
		for(i=0;i<MAX_DATA_FOR_BUF;i++){
	  		msG.message[i]='\0';
	  	}
	  	msG.fd=-1;
	  	msG.len=-1;
		
	  }
	
	printf("out oF WHile\n");
	getchar();
	/*read(conn->sock, &len, sizeof(int));
	if (len > 0)
	{
		addr = (long)((struct sockaddr_in *)&conn->address)->sin_addr.s_addr;
		buffer = (char *)malloc((len+1)*sizeof(char));
		buffer[len] = 0;

		/* read message */
		/*read(conn->sock, buffer, len);

		/* print message */
		/*printf("%d.%d.%d.%d: %s\n",
			(int)((addr      ) & 0xff),
			(int)((addr >>  8) & 0xff),
			(int)((addr >> 16) & 0xff),
			(int)((addr >> 24) & 0xff),
			buffer);
		free(buffer);
	}

	/* close socket and clean up */
	/*close(conn->sock);
	free(conn);*/
	pthread_exit(0);
}

int main(int argc, char ** argv)
{
    fd_set master;    // master file descriptor list
    fd_set read_fds;  // temp file descriptor list for select()
    int fdmax;        // maximum file descriptor number
    int len;
    
    int thread_pool_size=-1;//posa worker threads exoume
    int port=-1;//port
    
    int listener;     // listening socket descriptor
    int newfd;        // newly accept()ed socket descriptor
    struct sockaddr_storage remoteaddr; // client address
    socklen_t addrlen;

    char buf[MAX_DATA_FOR_BUF];    // buffer for client data
    int nbytes;
	
    char remoteIP[INET6_ADDRSTRLEN];
    
    int yes=1;        // for setsockopt() SO_REUSEADDR, below
    int i, j, rv,k;
    int err;
    struct client_message msG;//to mhnuma tou current client
    struct lista *pointer;
    
    
    struct addrinfo hints, *ai, *p;
    
    pthread_t * thread_matrix; /*pinakas me thread_pool_size deiktes se threads*/

    FD_ZERO(&master);    // clear the master and temp sets
    FD_ZERO(&read_fds);
    
    pthread_mutex_init(&mtx, 0);//initialize mutex
    pthread_cond_init(&cond, 0);//initialize condition variable
    
    /* check for command line arguments */
    if (argc != 3)
    {
	fprintf(stderr, "usage: %s port thread_pool_size\n", argv[0]);
	return -1;
    }
	
    /* obtain port number */
	if (sscanf(argv[1], "%d", &port) <= 0)
	{
		fprintf(stderr, "%s: error: wrong parameter: port thread_pool_size\n", argv[0]);
		return -2;
	}	
	
	thread_pool_size=atoi(argv[2]);
	
	//ftiaxnoume ta thread_pool_size threads
	thread_matrix=malloc( (thread_pool_size) * sizeof( pthread_t ) );
	for(i=0;i<thread_pool_size;i++){
		if(err=pthread_create(&(thread_matrix[i]), NULL, threadfunction, NULL)){
			perror("pthread_create");
			exit(1);
		}
		
	}
    // get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
        exit(1);
    }
    
    for(p = ai; p != NULL; p = p->ai_next) {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) { 
            continue;
        }
        
        // lose the pesky "address already in use" error message
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }

        break;
    }

    // if we got here, it means we didn't get bound
    if (p == NULL) {
        fprintf(stderr, "selectserver: failed to bind\n");
        exit(2);
    }

    freeaddrinfo(ai); // all done with this

    // listen
    if (listen(listener, 10) == -1) {
        perror("listen");
        exit(3);
    }

    // add the listener to the master set
    FD_SET(listener, &master);

    // keep track of the biggest file descriptor
    fdmax = listener; // so far, it's this one

    // main loop
    for(;;) {
        read_fds = master; // copy it
        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(4);
        }

        // run through the existing connections looking for data to read
        for(i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) { // we got one!!
                if (i == listener) {
                    // handle new connections
                    addrlen = sizeof remoteaddr;
                    newfd = accept(listener,
                        (struct sockaddr *)&remoteaddr,
                        &addrlen);

                    if (newfd == -1) {
                        perror("accept");
                    } else {
                        FD_SET(newfd, &master); // add to master set
                        if (newfd > fdmax) {    // keep track of the max
                            fdmax = newfd;
                        }
                        printf("selectserver: new connection from %s on "
                            "socket %d\n",
                            inet_ntop(remoteaddr.ss_family,
                                get_in_addr((struct sockaddr*)&remoteaddr),
                                remoteIP, INET6_ADDRSTRLEN),
                            newfd);
                    }
                } else {
                    // handle data from a client
                    if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
                        // got error or connection closed by client
                        if (nbytes == 0) {
                            // connection closed
                            printf("selectserver: socket %d hung up\n", i);
                        } else {
                            perror("recv");
                        }
                        close(i); // bye!
                        FD_CLR(i, &master); // remove from master set
                    } else {
                    	printf("exoume dedomena apo ton client %d buf=%s\n",i,buf);
                    	printf("nbytes=%d\n",nbytes);
                    	len=strlen(buf);
                    	msG.len=len;
                    	msG.fd=i;
                        memcpy(msG.message, buf,len);
                        pthread_mutex_lock(&mtx);
                        pointer=add_to_list(msG,1);
                        pthread_mutex_unlock(&mtx);
                        
                        printf("etoimoi na dwsoume to signal||||||||||||||||||||\n");
                        pthread_cond_signal(&cond);
                    	
                    	for(k=0;k<MAX_DATA_FOR_BUF;k++){
                    		buf[k]='\0';
                    		msG.message[k]='\0';
                    	}
                        // we got some data from a client
                       /* for(j = 0; j <= fdmax; j++) {
                        printf("eimaste panw apo to FD_iSSET i=%d, j=%d\n",i,j);
                        getchar();
                            // send to everyone!
                            if (FD_ISSET(j, &master)) {
                            printf("eimaste mesa sthn FD_iSSET i=%d, j=%d\n",i,j);
                            getchar();
                                // except the listener and ourselves
                                if (j != listener && j != i) {
                                printf("eimaste mesa sto if (j != listener && j != i) i=%d, j=%d\n",i,j);
                                getchar();
                                    if (send(j, buf, nbytes, 0) == -1) {
                                        perror("send");
                                    }
                                    if(j==listener){//edw mesa logika prepei na pairnei ena thread p einai se anamonh
                                    //kai na tou stelnei to mhnuma (buf) p tou xei steilei o client
                                    printf("eimaste mesa sto  if(j==listener) i=%d, j=%d\n",i,j);
                                    getchar();
                                    	
					
                                    	
                                    }
                                }
                            }
                        }*/
                    }
                } // END handle data from client
            } // END got new incoming connection
        } // END looping through file descriptors
    } // END for(;;)--and you thought it would never end!
    
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mtx);
    return 0;
}
