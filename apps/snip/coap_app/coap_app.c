/*
 * Broadcom Proprietary and Confidential. Copyright 2016 Broadcom
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 */

/** @file
 *
 * COAP Application
 *
 * This application snippet demonstrates how to react to COAP packet
 * from  network client and activate the services exported by Coap.
 *
 * Features demonstrated
 *  - Wi-Fi softAP mode
 *  - DHCP server
 *  - COAP receive / transmit
 *
 * Application Instructions
 *   1. Connect a PC terminal to the serial port of the WICED Eval board,
 *      then build and download the application as described in the WICED
 *      Quick Start Guide
 *   2. Connect your computer using Wi-Fi to "WICED COAP SoftAP"
 *        - SoftAP credentials are defined in wifi_config_dct.h
 *   3. Install Coper plug-in on top of Firefox from below location
 *   https://addons.mozilla.org/en-US/firefox/addon/copper-270430/
 *   4. The above plug-in helps to validate Coap server functions
 *
 *   When the Wi-Fi client (computer/Firfox cliet) joins the WICED softAP,
 *   it receives an IP address such as 192.168.0.2. To force
 *   the app to send COAP packets directly to the computer (rather than
 *   to a broadcast address), comment out the #define UDP_TARGET_IS_BROADCAST
 *   and change the target IP address to the IP address of your computer.
 *
 *   Open Firefox and try coap://192.168.0.1:5683/light and this will execute light on /off
 *   when we press PUT button along with data "1" (ON) or "0" (OFF), data must be available
 *   in the outgoing tab button on Firefox COAP plug-in
 *
 *
 */

#include "server/coap_server.h"

/******************************************************
 *                      Macros
 ******************************************************/

#define DTLS_ENABLE

#ifdef UDP_TARGET_IS_BROADCAST
#define UDP_TARGET_IP MAKE_IPV4_ADDRESS( 192,168,0,255 )
#else
#define UDP_TARGET_IP MAKE_IPV4_ADDRESS( 192,168,0,2 )
#endif

#ifdef DTLS_ENABLE
#define WICED_COAP_TARGET_PORT ( 5684 )
#else
#define WICED_COAP_TARGET_PORT ( 5683 )
#endif


#define CERTIFICATE_ECDSA_STRING  \
"-----BEGIN CERTIFICATE-----\r\n" \
"MIICHzCCAaWgAwIBAgIBCTAKBggqhkjOPQQDAjA+MQswCQYDVQQGEwJOTDERMA8G\r\n" \
"A1UEChMIUG9sYXJTU0wxHDAaBgNVBAMTE1BvbGFyc3NsIFRlc3QgRUMgQ0EwHhcN\r\n" \
"MTMwOTI0MTU1MjA0WhcNMjMwOTIyMTU1MjA0WjA0MQswCQYDVQQGEwJOTDERMA8G\r\n" \
"A1UEChMIUG9sYXJTU0wxEjAQBgNVBAMTCWxvY2FsaG9zdDBZMBMGByqGSM49AgEG\r\n" \
"CCqGSM49AwEHA0IABDfMVtl2CR5acj7HWS3/IG7ufPkGkXTQrRS192giWWKSTuUA\r\n" \
"2CMR/+ov0jRdXRa9iojCa3cNVc2KKg76Aci07f+jgZ0wgZowCQYDVR0TBAIwADAd\r\n" \
"BgNVHQ4EFgQUUGGlj9QH2deCAQzlZX+MY0anE74wbgYDVR0jBGcwZYAUnW0gJEkB\r\n" \
"PyvLeLUZvH4kydv7NnyhQqRAMD4xCzAJBgNVBAYTAk5MMREwDwYDVQQKEwhQb2xh\r\n" \
"clNTTDEcMBoGA1UEAxMTUG9sYXJzc2wgVGVzdCBFQyBDQYIJAMFD4n5iQ8zoMAoG\r\n" \
"CCqGSM49BAMCA2gAMGUCMQCaLFzXptui5WQN8LlO3ddh1hMxx6tzgLvT03MTVK2S\r\n" \
"C12r0Lz3ri/moSEpNZWqPjkCMCE2f53GXcYLqyfyJR078c/xNSUU5+Xxl7VZ414V\r\n" \
"fGa5kHvHARBPc8YAIVIqDvHH1Q==\r\n" \
"-----END CERTIFICATE-----\r\n"

#define PRIVATE_KEY_ECDSA_STRING  \
"-----BEGIN EC PRIVATE KEY-----\r\n" \
"MHcCAQEEIPEqEyB2AnCoPL/9U/YDHvdqXYbIogTywwyp6/UfDw6noAoGCCqGSM49\r\n" \
"AwEHoUQDQgAEN8xW2XYJHlpyPsdZLf8gbu58+QaRdNCtFLX3aCJZYpJO5QDYIxH/\r\n" \
"6i/SNF1dFr2KiMJrdw1VzYoqDvoByLTt/w==\r\n" \
"-----END EC PRIVATE KEY-----\r\n"



/******************************************************
 *                    Constants
 ******************************************************/

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
 *               Function Declarations
 ******************************************************/
wiced_result_t handle_light( void *context, wiced_coap_server_service_t* service, wiced_coap_server_request_t *request );

/******************************************************
 *               Variables Definitions
 ******************************************************/

static const wiced_ip_setting_t device_init_ip_settings =
{ INITIALISER_IPV4_ADDRESS( .ip_address, MAKE_IPV4_ADDRESS(192,168, 0, 1) ), INITIALISER_IPV4_ADDRESS( .netmask, MAKE_IPV4_ADDRESS(255,255,255, 0) ), INITIALISER_IPV4_ADDRESS( .gateway, MAKE_IPV4_ADDRESS(192,168, 0, 1) ), };

/******************************************************
 *               Function Definitions
 ******************************************************/
wiced_coap_server_t server;
wiced_coap_server_service_t service[ 7 ];
wiced_coap_security_t security;

static uint8_t laststatus;

void application_start( void )
{
    wiced_init( );

    wiced_network_up( WICED_AP_INTERFACE, WICED_USE_INTERNAL_DHCP_SERVER, &device_init_ip_settings );

    wiced_coap_server_init( &server );

//    security.cert = "Client_identity\0";
//    security.key  = "secretPSK\0";

    security.cert = CERTIFICATE_ECDSA_STRING;
    security.key  = PRIVATE_KEY_ECDSA_STRING;

    /* Start COAP server */
    wiced_coap_server_start( &server, WICED_AP_INTERFACE, WICED_COAP_TARGET_PORT, &security );

    if ( wiced_coap_server_add_service( &server, &service[ 0 ], "LIGHT", handle_light, WICED_COAP_CONTENTTYPE_TEXT_PLAIN ) != WICED_SUCCESS )
    {
        WPRINT_APP_INFO (("Error in adding service\n"));
        return;
    }

    if ( wiced_coap_server_add_service( &server, &service[ 1 ], "HOME/FAN", handle_light, WICED_COAP_CONTENTTYPE_TEXT_PLAIN ) != WICED_SUCCESS )
    {
        WPRINT_APP_INFO (("Error in adding service\n"));
        return;
    }

    if ( wiced_coap_server_add_service( &server, &service[ 2 ], "HOME/BULB", handle_light, WICED_COAP_CONTENTTYPE_TEXT_PLAIN ) != WICED_SUCCESS )
    {
        WPRINT_APP_INFO (("Error in adding service\n"));
        return;
    }

    if ( wiced_coap_server_add_service( &server, &service[ 3 ], "HOME/AC", handle_light, WICED_COAP_CONTENTTYPE_TEXT_PLAIN ) != WICED_SUCCESS )
    {
        WPRINT_APP_INFO (("Error in adding service\n"));
        return;
    }

    if ( wiced_coap_server_add_service( &server, &service[ 4 ], "HOME/FRIDGE", handle_light, WICED_COAP_CONTENTTYPE_TEXT_PLAIN ) != WICED_SUCCESS )
    {
        WPRINT_APP_INFO (("Error in adding service\n"));
        return;
    }

    if ( wiced_coap_server_add_service( &server, &service[ 5 ], "HOME/DOOR", handle_light, WICED_COAP_CONTENTTYPE_TEXT_PLAIN ) != WICED_SUCCESS )
    {
        WPRINT_APP_INFO (("Error in adding service\n"));
        return;
    }
}

wiced_result_t handle_light( void *context, wiced_coap_server_service_t* service, wiced_coap_server_request_t* request )
{
    static uint8_t light;
    wiced_coap_server_response_t response;
    wiced_coap_notification_type type = WICED_COAP_NOTIFICATION_TYPE_NONE;

    memset( &response, 0, sizeof( response ) );

    switch ( request->method )
    {
        case WICED_COAP_METHOD_GET:
            response.payload.data = &laststatus;
            response.payload.len = 1;
            break;

        case WICED_COAP_METHOD_POST:
            if ( request->payload.data[ 0 ] == '1' )
            {
                wiced_gpio_output_high( WICED_LED1 );
                light = 1;
            }
            else
            {
                wiced_gpio_output_low( WICED_LED1 );
                light = 0;
            }

            if ( laststatus != light )
            {
                type = WICED_COAP_NOTIFICATION_TYPE_CONFIRMABLE;
                laststatus = light;
                response.payload.data = &light;
                response.payload.len = 1;
            }
            break;

        case WICED_COAP_METHOD_DELETE:
            /* Free service only after sending response back to client */
            wiced_coap_server_delete_service( context, service );
            WPRINT_APP_INFO(( "service deleted successfully\n" ));
            break;

        default:
            WPRINT_APP_INFO(( "unknown method\n" ));
            break;
    }

    /* Send COAP response back to client */
    wiced_coap_server_send_response( context, service, request->req_handle, &response, type );

    return WICED_SUCCESS;
}