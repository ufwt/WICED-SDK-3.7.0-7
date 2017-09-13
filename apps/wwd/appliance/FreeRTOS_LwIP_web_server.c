/*
 * Broadcom Proprietary and Confidential. Copyright 2016 Broadcom
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 */

/**
 * @file
 *
 * This application provides an example of a simple web server
 * using FreeRTOS / LwIP.
 *
 * The application does not use a filesystem. All served pages are
 * generated by an associated content_[platform].c file
 *
 */

#include <stdlib.h>
#include "wwd_wifi.h"
#include "wwd_management.h"
#include "wwd_network.h"
#include "network/wwd_buffer_interface.h"
#include "platform/wwd_platform_interface.h"
#include "lwip/opt.h"
#include "lwip/mem.h"
#include <string.h>
#include "lwip/init.h"
#include "lwip/tcpip.h"
#include "lwip/sockets.h"  /* equivalent of <sys/socket.h> */
#include "lwip/inet.h"
#include "lwip/dhcp.h"
#include "wwd_wifi.h"
#include "wwd_management.h"
#include "network/wwd_buffer_interface.h"
#include "platform/wwd_platform_interface.h"
#include "RTOS/wwd_rtos_interface.h"
#include "web_server.h"
#include "wwd_debug.h"
#include "wwd_assert.h"
#include "wiced_utilities.h"
#include "platform_toolchain.h"

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/
#define ETHER_PAYLOAD_MTU (1500)
#define IP_HEADER_SIZE    (20)
#define TCP_HEADER_SIZE   (20)
/* Using the full MTU for TCP causes issues with LwIP trying to chain buffers */
/* #define MAX_TCP_PAYLOAD   ( ETHER_PAYLOAD_MTU - IP_HEADER_SIZE - TCP_HEADER_SIZE ) */
#define MAX_TCP_PAYLOAD 1200

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *               Static Function Declarations
 ******************************************************/

/******************************************************
 *               Variable Definitions
 ******************************************************/
static char                     pkt_buffer[300];
static int                      web_server_quit_flag = 1;
static xTaskHandle              web_server_thread_handle;
static const url_list_elem_t *  server_url_list;
static uint32_t                 server_local_addr;

/******************************************************
 *               Function Definitions
 ******************************************************/
static void web_server_proc( void * thread_input )
{
    run_webserver( server_local_addr, server_url_list );

    /* Clean up this startup thread */
    vTaskDelete( web_server_thread_handle );
}



void start_web_server_thread( uint32_t bind_address_in, const url_list_elem_t * server_url_list_in )
{
    server_local_addr = bind_address_in;
    server_url_list = server_url_list_in;
    web_server_quit_flag = 0;

    xTaskCreate( web_server_proc, "Web Server thread", WEB_SERVER_STACK_SIZE/sizeof( portSTACK_TYPE ), NULL, DEFAULT_THREAD_PRIO, &web_server_thread_handle );
}

void quit_web_server( void )
{
    web_server_quit_flag = 1;
}


wiced_bool_t web_server_is_running( void )
{
    return ( ( web_server_quit_flag != 1 )? WICED_TRUE : WICED_FALSE);
}



void run_webserver( uint32_t bind_address_in, const url_list_elem_t * server_url_list_in )
{
    int socket;
    struct sockaddr_in bind_address;
    bind_address.sin_addr.s_addr = (u32_t) bind_address_in;
    bind_address.sin_family = AF_INET;

    /* Create server socket */
    if ( 0 > ( socket = socket( AF_INET, SOCK_STREAM, 0 ) ) )
    {
        WEB_SERVER_ERROR_PRINT( ( "Failed to create server socket\n" ) );
        return;
    }

    /* Bind socket to port 80 (HTTP) */
    bind_address.sin_port = htons( 80 );

    if ( 0 > bind( socket, (struct sockaddr *) &bind_address,
            sizeof( struct sockaddr_in ) ) )
    {
        WEB_SERVER_ERROR_PRINT( ( "Failed bind server socket to port 80 (HTTP)\n" ) );
        return;

    }

    /* Listen for connections on server socket */
    int rc = listen( socket, 10 );
    if ( rc != 0 )
    {
        WEB_SERVER_ERROR_PRINT( ( "Failed to listen on server socket\n" ) );
        return;
    }

    /* set timeout for accepting connections - so that quit flag gets checked periodically */
    int recv_timeout = 500; /* milliseconds */
    setsockopt( socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&recv_timeout, sizeof( recv_timeout ) );

    web_server_quit_flag = 0;

    /* Loop forever, serving pages */
    WEB_SERVER_PRINT(("Waiting for page requests\n"));
    while ( web_server_quit_flag == 0 )
    {
        struct sockaddr_in remote_address;
        u32_t addr_length;

        /* Wait until a connection on the server socket is received */
        int accepted_socket_hnd = accept( socket, (struct sockaddr *) &remote_address, &addr_length );
        if ( accepted_socket_hnd < 0 )
        {
            continue;
        }

        /* Receive at most 200 bytes from connected socket (enough for request path, query and fragment id) */
        recv_timeout = 2000;
        setsockopt( accepted_socket_hnd, SOL_SOCKET, SO_RCVTIMEO, (char*)&recv_timeout, sizeof( recv_timeout ) );
        int bytes_received = recv( accepted_socket_hnd, pkt_buffer, sizeof( pkt_buffer ), 0 );
        if ( bytes_received > 0 )
        {
            char * request_string = pkt_buffer;
            char * start_of_url = request_string;
            char * end_of_url = request_string;

            /* Check that this is a GET request - other methods are not supported */
            if ( strncmp( request_string, "GET ", 4 ) == 0 )
            {
                request_string[3] = '\x00';
                start_of_url = &request_string[4];
            }
            if ( strncmp( request_string, "POST ", 5 ) == 0 )
            {
                request_string[4] = '\x00';
                start_of_url = &request_string[5];
            }

            end_of_url = start_of_url;
            /* Find the end of the request path ( space, newline, null, or end of packet ) */
            while ( ( *end_of_url != ' ' ) &&
                    ( *end_of_url != '\n' ) &&
                    ( *end_of_url != '\x00' ) &&
                    ( end_of_url < (char*) &pkt_buffer[sizeof( pkt_buffer )] ) )
            {
                end_of_url++;
            }


            const char lengthstr[] = "Content-Length: ";
            static const char start_of_data_str[] = {'\r', '\n', '\r', '\n'};
            char* lengthpos = NULL;
            char* end_pos = NULL;
            int header_len = 0;
            int content_len = 0;

            char* last_new_line = NULL;
            int search_len;
            char junk_buffer[300];


            char * read_buffer      = pkt_buffer;
            int    read_buffer_size = bytes_received;

            int total_read = bytes_received;

            /* Cycle through parts of the header looking for items of interest
             * At this stage we only need Content-Length
             */

            do
            {

                end_pos = (char*) memmem( read_buffer, read_buffer_size, start_of_data_str, sizeof(start_of_data_str)-1 );
                last_new_line = (char*) memrchr( read_buffer, '\n', read_buffer_size );

                search_len = (end_pos==NULL)? ((last_new_line==NULL)?read_buffer_size:last_new_line-read_buffer):MIN(last_new_line-read_buffer,end_pos-read_buffer);

                header_len += search_len;

                if ( ( lengthpos = (char*) memmem( read_buffer,  search_len, lengthstr, sizeof( lengthstr )-1 ) ) != NULL )
                {
                    content_len = atoi( lengthpos + sizeof( lengthstr ) - 1 );
                }

                if ( end_pos == NULL )
                {
                    memcpy( junk_buffer, &read_buffer[search_len], read_buffer_size - search_len );
                    read_buffer = junk_buffer;
                    read_buffer_size -= search_len;
                    bytes_received = recv( accepted_socket_hnd, &read_buffer[read_buffer_size], sizeof( junk_buffer )-read_buffer_size, 0 );
                    if ( bytes_received <= 0 )
                    {
                        break;
                    }
                    else
                    {
                        total_read += bytes_received;
                        read_buffer_size += bytes_received;
                    }
                }

            } while ( end_pos == NULL );



            if ( content_len != 0 )
            {
                int read_len = header_len + content_len - total_read;


                /* receive posted file or any other received bytes so that client is sent a FIN-ACK rather than a RST-ACK */
                do
                {
                    bytes_received = recv( accepted_socket_hnd, junk_buffer, sizeof( junk_buffer ), (read_len>0)?0:O_NONBLOCK );
                    if ( bytes_received > 0 )
                    {
                        read_len -= bytes_received;
                    }
                    else
                    {
                        break;
                    }
                } while ( read_len > 0 );
            }
            else
            {
                /* Flush any extra data */
                lwip_fcntl( accepted_socket_hnd, F_SETFL, O_NONBLOCK );
                bytes_received = recv( accepted_socket_hnd, junk_buffer, sizeof(junk_buffer), O_NONBLOCK );
                do
                {
                    bytes_received = recv( accepted_socket_hnd, junk_buffer, bytes_received, (bytes_received>0)?0:O_NONBLOCK );
                }
                while (bytes_received > 0);
                lwip_fcntl( accepted_socket_hnd, F_SETFL, 0 );
            }


            /* Get address of client */
            struct sockaddr_in other_end_addr_struct;
            socklen_t other_end_addr_len = sizeof( other_end_addr_struct );
            if ( 0 != getpeername( accepted_socket_hnd, (struct sockaddr *) &other_end_addr_struct, &other_end_addr_len ) )
            {
                WEB_SERVER_PRINT( ( "Error getting IP address of connected TCP client\n" ) );
            }
            uint32_t other_end_addr = other_end_addr_struct.sin_addr.s_addr;

            /* farm out processing of the request to a sub process */
            if ( 1 == process_url_request( server_url_list_in, start_of_url, (int) end_of_url - (int) start_of_url, (void*) accepted_socket_hnd, htonl(other_end_addr) ) )
            {
                web_server_quit_flag = 1;
            }

        }

        /* End connection */
        close( accepted_socket_hnd );

    }

    web_server_quit_flag = 1;
    close( socket );

}




void send_web_data( void * socket, unsigned char * data, unsigned long length )
{
    while ( length > 0 )
    {
        uint16_t packet_size = ( length > MAX_TCP_PAYLOAD ) ? MAX_TCP_PAYLOAD : length;

        /* Send the packet out! */
        int num_sent = send( (int)socket, data, packet_size, 0 );
        if ( num_sent < 0 )
        {
            WEB_SERVER_ERROR_PRINT( ( "Error sending packet\n" ) );
            return;
        }
        data += packet_size;
        length -= packet_size;
    }
}






