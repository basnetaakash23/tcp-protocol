/*

  ECHOSERV.C
  ==========
  (c) Paul Griffiths, 1999
  Email: mail@paulgriffiths.net
  
  Simple TCP/IP echo server.

*/


#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */

#include "converter.h"        /*  file conversion functions */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/*  Global constants  */

#define ECHO_PORT          (2002)
#define MAX_LINE           (1000)
#define LISTENQ            (1024)


int main(int argc, char *argv[]) {
    int       list_s;                /*  listening socket          */
    int       conn_s;                /*  connection socket         */
    short int port;                  /*  port number               */
    struct    sockaddr_in servaddr;  /*  socket address structure  */
    char      buffer[MAX_LINE];      /*  character buffer          */
    char     *endptr;                /*  for strtol()              */
    int       status = 0;


        /*  Get port number from the command line, and
        set to default port if no arguments were supplied  */

    if ( argc == 2 ) {
        port = strtol(argv[1], &endptr, 0);
	if ( *endptr ) {
	    fprintf(stderr, "ECHOSERV: Invalid port number.\n");
	    exit(EXIT_FAILURE);
	}
    }
    else if ( argc < 2 ) {
        port = ECHO_PORT;
    }
    else {
        fprintf(stderr, "ECHOSERV: Invalid arguments.\n");
        exit(EXIT_FAILURE);
    }

	
        /*  Create the listening socket  */

    if ( (list_s = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        fprintf(stderr, "ECHOSERV: Error creating listening socket.\n");
        exit(EXIT_FAILURE);
    }


        /*  Set all bytes in socket address structure to
        zero, and fill in the relevant data members   */

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(port);


        /*  Bind our socket addresss to the 
	   listening socket, and call listen()  */

    if ( bind(list_s, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
        fprintf(stderr, "ECHOSERV: Error calling bind()\n");
        exit(EXIT_FAILURE);
    }

    if ( listen(list_s, LISTENQ) < 0 ) {
        fprintf(stderr, "ECHOSERV: Error calling listen()\n");
        exit(EXIT_FAILURE);
    }

    
        /*  Enter an infinite loop to respond
        to client requests and echo input  */

    while ( 1 ) {

	   /*  Wait for a connection, then accept() it  */

	if ( (conn_s = accept(list_s, NULL, NULL) ) < 0 ) {
	    fprintf(stderr, "ECHOSERV: Error calling accept()\n");
	    exit(EXIT_FAILURE);
	}


	    /* Retrieve an input line from the connected socket
	    then simply write it back to the same socket.     */
       
        //Writeline(conn_s, &size, sizeof(size));
        char new_buffer[1000];
        read(conn_s, new_buffer, MAX_LINE);
        // uint8_t file_length = new_buffer[0];
        // printf("The file.... length is %d\n", file_length);
        
        /*Getting the format type */
        char format[2] ;
        format[0] = new_buffer[0];
        format[1] = '\0';

        //printf("%c\n", new_buffer[0]);
        printf("The format is %s\n",format);
        //checking if the input is in correct format
        if(strstr("0123", format) == NULL){
            printf("Format type not correct\n");
            status = -1;
            //exit(EXIT_FAILURE);
        }
        uint8_t file_name_length = new_buffer[1];
        
        /*NOW getting the length of filename*/
        
        char target_file[file_name_length+1];
        memcpy(target_file, new_buffer+2, file_name_length);
        target_file[file_name_length] = '\0';

        
        // uint8_t file_length = new_buffer[2+file_name_length];
        // printf("The file.... length is %d\n", file_length);
        char filelength[4];
        memcpy(filelength,new_buffer+2+file_name_length,4);
        int file_length = *filelength;

        char file_content[file_length] ;
        memcpy(file_content, new_buffer+6+file_name_length,file_length);
        
       for(int i = 0; i< file_length; i++){
           printf("%d. The character member is %d\n",i+1, file_content[i]);
       }
        //created file pointer for the target file 
                //sending the file to the process file 

        process_file(file_content, file_length, target_file, format);
        printf("Process file called\n");
        
        //write(conn_s, new_buffer, file_length);
        //        printf("%d\n", status);
        
        //        Readline(conn_s, buffer, 10);
        //
        //        Writeline(conn_s, buffer, 10);
        //Sending confirmation message back to the client
        char connection_status[100] = {'F','o','r','m','a','t',' ','E','r','r','o','r','\0'};
        char connection_status1[100] = {'F','i','l','e',' ','T','r','a','n','s','l','a','t','e','d','a','n','d',' ','s','a','v','e','d','\0'};
        
        if(status < 0){
            write(conn_s, connection_status, 100);
            
        }
        else{
            write(conn_s,connection_status1,100);
        }
        
	/*  Close the connected socket  */

	if ( close(conn_s) < 0 ) {
	    fprintf(stderr, "ECHOSERV: Error calling close()\n");
	    exit(EXIT_FAILURE);
	}
    }
}
