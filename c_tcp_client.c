#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define SERVER_IP_ADDR "localhost"
#define SERVER_PORT 5000
#define MAXBUFF 1000

int get_send_line(char s[], int lim);

int main()
{
	int real_server_message_length; /* Holds the real length of the server message, without extra characters. */
	int len; /* Holds return value of get_send_line when the user fetches a value. */
	char line[MAXBUFF]; /* This is where get_send_line outputs. */
	int socket_descriptor = 0; /* A file descriptor int corresponding to the client socket. */
	int connect_status = 0, send_status = 0, recv_status = 0; /* Various variables to hold return values of system calls. */
	int connection_terminated = 0; /* A boolean that is set by user when they want to exit the program. */
	char server_reply[MAXBUFF]; /* This is where recv system calls output when we get server response. */
	struct sockaddr_in serv_addr; /* A struct that is used to represent the server address. */

	socket_descriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERVER_PORT);
	inet_aton(SERVER_IP_ADDR, &serv_addr.sin_addr);
	
	connect_status = connect(socket_descriptor, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in));

	if (connect_status == 0)
	{
		printf("Successfully connected!\n");
	}
	else
	{
		printf("Failed connection!\n");
	}

	while (!connection_terminated)
	{
		printf("Enter input to server, or write \"quit()\" to exit program.");
		len = get_send_line(line, MAXBUFF);
		if (len > 0)
		{
			if (strcmp(line, "quit()") != 0)
			{
				connection_terminated = 1;
				continue;
			}
			send_status = send(socket_descriptor, line, len, 0);
			printf("Send status: %d\n", send_status);
			if (send_status > 0)
			{
				recv_status = recv(socket_descriptor, server_reply, MAXBUFF, 0);
				if (recv_status > 0)
				{
					real_server_message_length = strlen(server_reply) - strlen("Message received: ");
					printf("Server response length: %d\n", real_server_message_length); 
					printf("Server:%s\n", server_reply);
				}
			}
			else
			{
				printf("Error when writing to server\n");
			}
		}
	}

	/* Shutdown socket before we exit the program.*/
	close(socket_descriptor);
}

/* get_send_line: A simple function for fetching a line from stdin, props 
 * go to K&R programming for this little gem. */
int get_send_line(char s[], int lim)
{
	int c, i;

	for (i=0; i<lim-1 && (c=getchar())!=EOF && c!='\n'; ++i)
		s[i] = c;
	
	if (c == '\n') {
		s[i] = c;
		++i;
	}
	
	s[i] = '\0';
	
	return i;
}
