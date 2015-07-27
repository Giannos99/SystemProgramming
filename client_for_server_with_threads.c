#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char ** argv)
{
	int port;
	int sock = -1;
	struct sockaddr_in address;
	struct hostent * host;
	int len;
	char buf[1000];
	
	/* checking commandline parameter */
	if (argc != 4)
	{
		printf("usage: %s hostname port text\n", argv[0]);
		return -1;
	}
	
	/* obtain port number */
	if (sscanf(argv[2], "%d", &port) <= 0)
	{
		fprintf(stderr, "%s: error: wrong parameter: port\n", argv[0]);
		return -2;
	}
	
	/* create socket */
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock <= 0)
	{
		fprintf(stderr, "%s: error: cannot create socket\n", argv[0]);
		return -3;
	}
	
	/* connect to server */
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	host = gethostbyname(argv[1]);
	if (!host)
	{
		fprintf(stderr, "%s: error: unknown host %s\n", argv[0], argv[1]);
		return -4;
	}
	memcpy(&address.sin_addr, host->h_addr_list[0], host->h_length);
	if (connect(sock, (struct sockaddr *)&address, sizeof(address)))
	{
		fprintf(stderr, "%s: error: cannot connect to host %s\n", argv[0], argv[1]);
		return -5;
	}

	/* send text to server */
	strcpy(buf,argv[3]);
	while(1){
		printf("Enter message : ");
		fgets (buf, sizeof(buf), stdin);

	   	/* Remove trailing newline, if there. */
	    	if ((strlen(buf)>0) && (buf[strlen (buf) - 1] == '\n'))
			buf[strlen (buf) - 1] = '\0';
        	//scanf("%[ AZaz09]" , buf);
         
        
		len = strlen(buf);
		printf("len=%d buf=%s\n",len,buf);
		
		write(sock, &len, sizeof(int));
		//Send some data
        	/*if( send(sock , buf , len , 0) < 0)
        	{
            		puts("Send failed");
            		return 1;
       		 }*/
		write(sock, buf, len);
		//scanf("What else do you want to tell?\n%s",buf);
		/*getchar();*/
	}

	/* close socket */
	close(sock);

	return 0;
}

