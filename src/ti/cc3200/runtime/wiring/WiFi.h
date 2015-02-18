#ifndef WIRING_WIFI_H
#define WIRING_WIFI_H

extern "C"{
#include <stdint.h>
#include <ti/drivers/WiFi.h>
#include <ti/sysbios/gates/GateMutex.h>
#include "netcfg.h"
}

#include "IPAddress.h"

/*
 * A TI-RTOS version of Arduino's WiFi class.
 * See: https://github.com/arduino/Arduino/blob/master/libraries/WiFi/WiFi.h
 * See: http://arduino.cc/en/Reference/WiFi
 */

// ==================================================== wl definitions; see:
// https://github.com/arduino/Arduino/blob/master/libraries/WiFi/utility/wl_definitions.h

#define WL_SSID_MAX_LENGTH          32              // Maximum size of a SSID
#define WL_WPA_KEY_MAX_LENGTH       63              // Length of passphrase. Valid lengths are 8-63.
#define WL_WEP_KEY_MAX_LENGTH       13              // Length of key in bytes. Valid values are 5 and 13.
#define WL_MAC_ADDR_LENGTH          SL_MAC_ADDR_LEN // Size of a MAC-address or BSSID
#define WL_IPV4_LENGTH              4               // Size of a MAC-address or BSSID
#define WL_NETWORKS_LIST_MAXNUM     10              // Maximum size of a SSID list
#define WL_MAX_ATTEMPT_CONNECTION   10              // Maximum number of attempts to establish wifi connection
#define MAX_SOCK_NUM                4               // Maxmium number of socket
#define NA_STATE                    -1              // Default state value for Wifi state field

typedef enum {
        WL_NO_SHIELD = 255,
        WL_IDLE_STATUS = 0,
        WL_NO_SSID_AVAIL,
        WL_SCAN_COMPLETED,
        WL_CONNECTED,
        WL_CONNECT_FAILED,
        WL_CONNECTION_LOST,
        WL_DISCONNECTED
} wl_status_t;

/* Encryption modes */
enum wl_enc_type {  /* Values map to 802.11 encryption suites... */
        ENC_TYPE_WEP  = 5,  // WEP
        ENC_TYPE_TKIP = 2,  // WPA
        ENC_TYPE_CCMP = 4,  // WPA2
        /* ... except these two, 7 and 8 are reserved in 802.11-2007 */
        ENC_TYPE_NONE = 7,
        ENC_TYPE_AUTO = 8
};

// ==================================================== end wl definitions

class WiFiClass{
    private:
        GateMutex_Handle _initMutex;
        void init();

        static WiFi_Handle _wifi;

        Sl_WlanNetworkEntry_t _networks[WL_NETWORKS_LIST_MAXNUM];

    public:
        WiFiClass();

        /* Start WiFi connection for OPEN networks
         *
         * param ssid: Pointer to the SSID string
         */
        int begin(char*ssid);

        /* Start Wifi connection with WEP encryption.
         * Configure a key into the device. The key type (WEP-40, WEP-104)
         * is determined by the size of the key (5 bytes for WEP-40, 13 bytes for WEP-104).
         *
         * param ssid: Pointer to the SSID string.
         * param key_idx: The key index to set. Valid values are 0-3. TODO don't ignore this
         * param key: Key input buffer.
         */
        int begin(char* ssid, uint8_t key_idx, char* key);

        /* Start Wifi connection with passphrase
         * In original Wiring, the most secure supported mode will be automatically selected.
         * TODO make that actually happen. Right now it's just WPA
         *
         * param ssid: Pointer to the SSID string.
         * param passphrase: Passphrase. Valid characters in a passphrase
         *        must be between ASCII 32-126 (decimal).
         */
        int begin(char* ssid, char *passphrase);

        /*
         * Disconnect from the network
         *
         * return: one value of wl_status_t enum
         */
        int disconnect(void);

         /*
         * Get the interface MAC address.
         *
         * param buf: array with length WL_MAC_ADDR_LENGTH, to hold MAC address
         * return: pointer to uint8_t array with length WL_MAC_ADDR_LENGTH
         */
        uint8_t *macAddress(uint8_t* buf);

        /*
         * Return the current SSID associated with the network
         *
         * return: ssid string
         */
        char *SSID();

        /*
         * Return the current BSSID associated with the network.
         * It is the MAC address of the Access Point
         *
         * return: pointer to uint8_t array with length WL_MAC_ADDR_LENGTH
         */
        uint8_t *BSSID(uint8_t* bssid);

        /*
         * Return the current RSSI /Received Signal Strength in dBm)
         * associated with the network
         *
         * return: signed value
         */
        int32_t RSSI();

        /*
         * Scan for available WiFi networks and return the discovered number
         * return: signed byte. Negative on error.
         */
        int8_t scanNetworks();

        /*
         * Return the SSID discovered during the network scan.
         *
         * param networkItem: specify from which network item want to get the information
         *
         * return: ssid string of the specified item on the networks scanned list
         */
        char *SSID(uint8_t networkItem);

        /*
         * Return the encryption type of the networks discovered during the scanNetworks
         *
         * param networkItem: specify from which network item want to get the information
         *
         * return: encryption type (enum wl_enc_type) of the specified item on the networks scanned list
         *
         * warning: the only values currently supported for return here are ENT_TYPE_NONE,
         * ENT_TYPE_WEP, and ENT_TYPE_TKIP (WPA), and ENT_TYPE_CCMP (WPA2). TODO?
         */
        uint8_t encryptionType(uint8_t networkItem);

        /*
         * Return the RSSI of the networks discovered during the scanNetworks
         *
         * param networkItem: specify from which network item want to get the information
         *
         * return: signed value of RSSI of the specified item on the networks scanned list
         */
        int32_t RSSI(uint8_t networkItem);

        /*
         * Return Connection status.
         *
         * return: one of the values defined in wl_status_t. This implementation
         * only returns WL_CONNECTED, WL_DISCONNECTED, or WL_IDLE_STATUS.
         *
         * warning: though widely used in Arduino examples, this function is not
         * documented in the official reference [http://arduino.cc/en/Reference/WiFi].
         * Because of this, I can only guess at how it's supposed to behave.
         *
         * note: since the "WiFi shield" of the CC3200 is on the same chip as the main
         * processor, this function will never return WL_NO_SHIELD.
         */
        uint8_t status();

        /*
         * Resolve the given hostname to an IP address.
         * param aHostname: Name to be resolved
         * param aResult: IPAddress structure to store the returned IP address
         * result: 1 if aIPAddrString was successfully converted to an IP address,
         *          else error code
         */
        int hostByName(const char* aHostname, IPAddress& aResult);

        /* Change Ip configuration settings disabling the dhcp client
         *
         * param local_ip:  Static ip configuration
         */
        void config(IPAddress local_ip);

        /* Change Ip configuration settings disabling the dhcp client
         *
         * param local_ip:  Static ip configuration
         * param dns_server:     IP configuration for DNS server 1
         */
        void config(IPAddress local_ip, IPAddress dns_server);

        /* Change Ip configuration settings disabling the dhcp client
         *
         * param local_ip:  Static ip configuration
         * param dns_server:     IP configuration for DNS server 1
         * param gateway :  Static gateway configuration
         */
        void config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway);

        /* Change Ip configuration settings disabling the dhcp client
         *
         * param local_ip:  Static ip configuration
         * param dns_server:     IP configuration for DNS server 1
         * param gateway:   Static gateway configuration
         * param subnet:        Static Subnet mask
         */
        void config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet);

        /* Gets the CC3200's current IP address.
         */
        IPAddress localIP();

        /* Gets the CC3200's current subnet mask.
         */
        IPAddress subnetMask();

        /* Gets the CC3200's current gateway IP address.
         */
        IPAddress gatewayIP();

        /*
         * NOTE/TODO: Not all WiFi functions are fully implemented, and some are still completely
         * absent. The functions mentioned at http://arduino.cc/en/Reference/WiFi that are members
         * of the WiFiClass class and are not yet implemented here are listed below:
         *      int  getSocket();
         *      void setDNS(IPAddress dns1);
         *      void setDNS(IPAddress dns1, IPAddress dns2);
         *      uint8_t encryptionType();
         *
         * Note that this refers only to functions listed under WiFi or IPAddress; Server, Client,
         * and UDP have not yet been implemented. TODO?
         */
};

extern WiFiClass WiFi;

#endif
