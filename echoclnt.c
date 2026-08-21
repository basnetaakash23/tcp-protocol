
#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */

#include "helper.h"           /*  Our own helper functions  */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>

void send_all(int socket, char *buffer, long length);

/*  Global constants  */

#define MAX_LINE           (1000)


/*  main()  */

int main(int argc, char *argv[]) {

    int       conn_s;                /*  connection socket         */
    short int port;                  /*  port number               */
    struct    sockaddr_in servaddr;  /*  socket address structure  */
    char      buffer[MAX_LINE];      /*  character buffer          */
    char     *szAddress;             /*  Holds remote IP address   */
    char     *szPort;                /*  Holds remote port         */
    char     *endptr;                /*  for strtol()              */
    char     *filepath;              /*  Holds the file path       */
    char     *toFormat;              /*  Holds the value of target file conversion format*/
    char     *toName;                /*  Holds the name for targetfile */


    /*  Get command line arguments  */
    if(argc != 6){
        printf("Incomplete Command Line arguments\n");
        exit(EXIT_FAILURE);
        return -1;
    }
    szAddress = argv[1];
    szPort = argv[2];
    filepath = argv[3];
    toFormat = argv[4];
    toName = argv[5];
    
    /*  Set the remote port  */

    port = strtol(szPort, &endptr, 0);
    if ( *endptr ) {
	printf("ECHOCLNT: Invalid port supplied.\n");
	exit(EXIT_FAILURE);
    }
	
    /*  Create the listening socket  */

    if ( (conn_s = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
	fprintf(stderr, "ECHOCLNT: Error creating listening socket.\n");
	exit(EXIT_FAILURE);
    }

    /*  Set all bytes in socket address structure to
        zero, and fill in the relevant data members   */

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_port        = htons(port);


    /*  Set the remote IP address  */

    if ( inet_aton(szAddress, &servaddr.sin_addr) <= 0 ) {
	printf("ECHOCLNT: Invalid remote IP address.\n");
	exit(EXIT_FAILURE);
    }

    
    /*  connect() to the remote echo server  */

    if ( connect(conn_s, (struct sockaddr *) &servaddr, sizeof(servaddr) ) < 0 ) {
	printf("ECHOCLNT: Error calling connect()\n");
	exit(EXIT_FAILURE);
    }

    
    //open the file that we are reading from
    FILE* fp = fopen(filepath,"rb");
    if (fp == NULL) {
        perror("Could not open file");
        return EXIT_FAILURE;
    }
    fseek(fp,0,SEEK_END);
    int file_length = ftell(fp);
    rewind(fp);


    printf("%d\n", file_length);


    char *file_buffer = malloc(file_length);
    if(file_buffer == NULL){
        printf("Memory allocation failed\n");
        return EXIT_FAILURE;
    }


    int num_of_items = fread(file_buffer, 1, file_length, fp);
    fclose(fp);
    
    //printing the contents of file
    for (int j=0; j<num_of_items; j++) {
        printf("%d == %d\n", j+1,file_buffer[j]);
    }

    //printf("%d\n", num_of_items);

    if (num_of_items <= 1) {
        printf("Error reading file\n");
    }

    if (strlen(toName) > 255) {
        printf("Filename is too long\n");
        return EXIT_FAILURE;
    }

    uint8_t filename_length = strlen(toName);

    //printf("%c\n", file_buffer[0]);
    printf("%d the filename length\n",filename_length);
    uint32_t network_file_length = htonl((uint32_t)file_length);

    //copying the address of meta information of the file and the required specification to the server
    char header[6 + 255];
    
    memcpy(header, toFormat, 1);
    memcpy(header + 1, &filename_length, 1);
    memcpy(header + 2, toName, filename_length);
    memcpy(header + 2 + filename_length,
        &network_file_length,
        sizeof(network_file_length));

    send_all(conn_s, header, 6 + filename_length);
    send_all(conn_s, file_buffer, file_length);

    free(file_buffer);

    
    /*  Send string to echo server, and retrieve response  */
    char connection_response[100];
   
    read(conn_s, connection_response,100);
    /*  Output echoed string  */

    printf("Echo response: %s \n", connection_response);
    return EXIT_SUCCESS;
}

void send_all(int socket, char *buffer, long length) {
    long sent = 0;

    while (sent < length) {
        int result = write(socket, buffer + sent, length - sent);

        if (result <= 0) {
            printf("Error sending file\n");
            return;
        }

        sent += result;
    }
}






