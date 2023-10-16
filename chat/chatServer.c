
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include "chatServer.h"

static int end_server = 0;

void intHandler(int SIG_INT) {
    /* use a flag to end_server to break the main loop */
    end_server = 1;
}

int main (int argc, char *argv[]) {

    signal(SIGINT, intHandler);

    conn_pool_t *pool = malloc(sizeof(conn_pool_t));
    init_pool(pool);

    /*************************************************************/
    /* Create an AF_INET stream socket to receive incoming      */
    /* connections on                                            */
    /*************************************************************/
    if (argc != 2 || (int) atoi(argv[1]) < 1 || (int) atoi(argv[1]) > 65536) {
        printf("Usage: server <port>");
        return 0;
    }

    struct sockaddr_in serv;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(atoi(argv[1]));
    serv.sin_addr.s_addr = htonl(INADDR_ANY);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket");
        exit(1);
    }
    /*************************************************************/
    /* Set socket to be nonblocking. All of the sockets for      */
    /* the incoming connections will also be nonblocking since   */
    /* they will inherit that state from the listening socket.   */
    /*************************************************************/
    int on = 1;
    ioctl(sock, (int) FIONBIO, (char *) &on);
    /*************************************************************/
    /* Bind the socket                                           */
    /*************************************************************/
    int bin = bind(sock, (struct sockaddr*) &serv, sizeof(serv));
    if (bin < 0) {
        perror("Bind");
        exit(1);
    }

    /*************************************************************/
    /* Set the listen back log                                   */
    /*************************************************************/
    int lis = listen(sock, 5);
    if (lis < 0) {
        perror("listen");
        exit(1);
    }

    /*************************************************************/
    /* Initialize fd_sets  			                             */
    /*************************************************************/
    FD_SET(sock, &pool->read_set);
    /*************************************************************/
    /* Loop waiting for incoming connects, for incoming data or  */
    /* to write data, on any of the connected sockets.           */
    /*************************************************************/
    do {
        conn_t *ptr=pool->conn_head;
        while(ptr != NULL){
            if(ptr->fd > pool->maxfd){
                pool->maxfd=ptr->fd;
            }
            ptr=ptr->next;
        }
        if(pool->maxfd<sock){
            pool->maxfd=sock;
        }
        /**********************************************************/
        /* Copy the master fd_set over to the working fd_set.     */
        /**********************************************************/
        pool->ready_write_set = pool->write_set;
        pool->ready_read_set = pool->read_set;
        /**********************************************************/
        /* Call select() 										  */
        /**********************************************************/
        printf("Waiting on select()...\nMaxFd %d\n", pool->maxfd);
        if ((pool->nready = select(pool->maxfd + 1, &pool->ready_read_set, &pool->ready_write_set, NULL, NULL)) < 0) {
            break;
        }


        /**********************************************************/
        /* One or more descriptors are readable or writable.      */
        /* Need to determine which ones they are.                 */
        /**********************************************************/

        for (/* check all descriptors, stop when checked all valid fds */
                int i = 0; i < pool->maxfd + 1 && pool->nready > 0; i++) {
            /* Each time a ready descriptor is found, one less has  */
            /* to be looked for.  This is being done so that we     */
            /* can stop looking at the working set once we have     */
            /* found all of the descriptors that were ready         */

            /*******************************************************/
            /* Check to see if this descriptor is ready for read   */
            /*******************************************************/

            if (FD_ISSET(i, &pool->ready_read_set)) {
                char buff[BUFFER_SIZE];
                 memset(buff, 0, BUFFER_SIZE);
                /***************************************************/
                /* A descriptor was found that was readable		   */
                /* if this is the listening socket, accept one      */
                /* incoming connection that is queued up on the     */
                /*  listening socket before we loop back and call   */
                /* select again. 						            */
                /****************************************************/

                if (i == sock) {
                    struct sockaddr_in cli;/* returned by accept()*/
                    int cli_len = sizeof(cli);
                    int new_sd = accept(i, (struct sockaddr *) &cli, (socklen_t *) &cli_len);
                    if (new_sd < 0) {
                        perror("accept");
                        exit(1);
                    } else {
                        /* add the new connection to the pool */

                        printf("New incoming connection on sd %d\n", new_sd);
                        pool->nready--;
                        add_conn(new_sd, pool);
                    }
                }

                    /****************************************************/
                    /* If this is not the listening socket, an 			*/
                    /* existing connection must be readable				*/
                    /* Receive incoming data his socket             */
                    /****************************************************/
                else {
                    printf("Descriptor %d is readable\n", i);
                    int chk = (int) read(i, buff, BUFFER_SIZE);
                    printf("%d bytes received from sd %d\n", (int) strlen(buff), i);
                    pool->nready--;
                    /* If the connection has been closed by client        */
                    /* remove the connection (remove_conn(...))           */
                    if (chk == 0) {
                        printf("Connection closed for sd %d\n", i);
                        remove_conn(i, pool);
                    }
                    /* If the connection has been closed by client 		*/
                    /* remove the connection (remove_conn(...))    		*/

                    /**********************************************/
                    /* Data was received, add msg to all other    */
                    /* connectios					  			  */
                    /**********************************************/
                    add_msg(i, buff, BUFFER_SIZE, pool);

                }
            }
            /* End of if (FD_ISSET()) */

            /*******************************************************/
            /* Check to see if this descriptor is ready for write  */
            /*******************************************************/
            if (FD_ISSET(i, &pool->ready_write_set)) {
                /* try to write all msgs in queue to sd */
                write_to_client(i, pool);
                pool->nready--;
            }
            /*******************************************************/


        } /* End of loop through selectable descriptors */


    }
    while (end_server == 0);
    int i = 0;
    int size = (int) pool->nr_conns;//how many active connections are still available.
    int removeCon[pool->nr_conns];// int array that holds all the active connections sd.

    conn_t *curr = pool->conn_head;
    while (curr != NULL) {
        removeCon[i] = curr->fd;
        curr = curr->next;
        i++;
    }
    for (i = size; i > 0; i--) {
        remove_conn(removeCon[i - 1], pool);
    }
    free(pool);
    /*************************************************************/
    /* If we are here, Control-C was typed,						 */
    /* clean up all open connections					         */
    /*************************************************************/

    return 0;
}



int init_pool(conn_pool_t *pool) {
    //initialized all fields
    pool->maxfd = 0;
    pool->nready = 0;
    FD_ZERO(&pool->read_set);
    FD_ZERO(&pool->write_set);
    FD_ZERO(&pool->ready_read_set);
    FD_ZERO(&pool->ready_write_set);
    pool->nr_conns = 0;
    pool->conn_head = NULL;
    return 0;
}

int add_conn(int sd, conn_pool_t *pool) {
    conn_t *connection = (conn_t *) malloc(sizeof(conn_t));
    if (connection == NULL)
        return -1;
    connection->next = NULL;
    connection->prev = NULL;
    connection->fd = sd;
    connection->write_msg_head = NULL;
    connection->write_msg_tail = NULL;
    if (pool->conn_head != NULL) {
        connection->next = pool->conn_head;
        pool->conn_head->prev = connection;
    }
    pool->conn_head = connection;
    FD_SET(sd, &pool->read_set);
    pool->nr_conns++;
    /*
     * 1. allocate connection and init fields
     * 2. add connection to pool
     * */
    return 0;
}


int remove_conn(int sd, conn_pool_t *pool) {
    /* 1. remove connection from pool */
    /* 2. deallocate connection */
    /* 3. remove from sets */
    /* 4. update max_fd if needed */

    printf("removing connection with sd %d \n", sd);

    conn_t *conn;
    conn_t *removed = NULL;
    for (conn = pool->conn_head; conn != NULL; conn = conn->next) {
        if (conn->fd == sd) {
            removed = conn;
            break;
        }
    }
    if (conn == NULL) {
        return -1;
    }
    if(removed == pool->conn_head) {
        pool->conn_head = removed->next;
    }
    if (removed->prev != NULL) {
        removed->prev->next = removed->next;
    }
    if (removed->next != NULL) {
        removed->next->prev = removed->prev;
    }

    FD_CLR(sd, &pool->read_set);
    FD_CLR(sd, &pool->write_set);
    pool->nr_conns--;
close(sd);
free(removed);


    return 0;
}


int add_msg(int sd, char *buffer, int len, conn_pool_t *pool) {

    /*
     * 1. add msg_t to write queue of all other connections
     * 2. set each fd to check if ready to write
     */

    conn_t *conn;
    msg_t *msg;
    for (conn = pool->conn_head; conn != NULL; conn = conn->next) {
        if (conn->fd != sd) {
            msg = (msg_t *) malloc(sizeof(msg_t));
            if(msg == NULL) {
                return -1;
            }
            msg->prev = NULL;
            msg->next = NULL;
            msg->message = (char *) malloc((len+1) * sizeof(char));
            if(msg->message == NULL){
                return -1;
            }
            memset(msg->message, 0, len+1);
            strcpy(msg->message, buffer);

            msg->size = len;
            if (conn->write_msg_head == NULL) {
                conn->write_msg_head = msg;
            } else {
                msg_t *tmpMSG=conn->write_msg_head;
                while(tmpMSG->next != NULL){
                    tmpMSG=tmpMSG->next;
                }
                tmpMSG->next=msg;
                msg->prev=conn->write_msg_tail;
                msg->next=NULL;
            }
            conn->write_msg_tail = msg;

            FD_SET(conn->fd, &pool->write_set);
        }
    }
    return 0;
}


int write_to_client(int sd, conn_pool_t *pool) {

    /*
     * 1. write all msgs in queue
     * 2. deallocate each writen msg
     * 3. if all msgs were writen successfully, there is nothing else to write to this fd... */


    conn_t *conn;
    msg_t *msg;
    int n;

    for (conn = pool->conn_head; conn != NULL; conn = conn->next) {
        if (conn->fd == sd) {
            break;
        }
    }
    if (conn == NULL) {
        return -1;
    }

    while (conn->write_msg_head != NULL) {
        msg = conn->write_msg_head;
        n = write(sd, msg->message, msg->size);
        if (n < 0) {
            return -1;
        } else if (n < msg->size) {
            break;
        }

        conn->write_msg_head = msg->next;
        free(msg->message);
        free(msg);
    }

        FD_CLR(sd, &pool->write_set);
    conn->write_msg_head = NULL;
    conn->write_msg_tail = NULL;

    return 0;
}


