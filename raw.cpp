#include "IPv4.h"
#include "IPv6.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sys/socket.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <arpa/inet.h>
#include <errno.h>
#include <cstdlib>
#include <iomanip>

std::ofstream ipv4_writer("ipv4.txt", std::fstream::app);
std::ofstream ipv6_writer("ipv6.txt", std::fstream::app);

// Write the source and destination MAC addresses to file.
void write_mac_address(Packet packet, std::ofstream& writer){ // Need reference to the writer stream's buffer.

    int i;

    writer << "SOURCE_MAC: ";

    for (i=1;i<=sizeof(packet.source_mac);i++) { // Write the source MAC address in hexadecimal, padded with zeroes
        writer << std::setfill('0') << std::setw(2) << std::hex << (int)packet.source_mac[i-1];
        if (i<(sizeof(packet.source_mac))) {
            writer << ":";
        }
    }

    writer << "\n";

    writer << "DEST_MAC: ";

    for (i=1;i<=sizeof(packet.dest_mac);i++) { // Write the destination MAC address in hexadecimal, padded with zeroes
        writer << std::setfill('0') << std::setw(2) << std::hex << (int)packet.dest_mac[i-1];
        if (i<(sizeof(packet.dest_mac))) {
            writer << ":";
        }
    }

    writer << "\n";

}

// Write relevant IPv4 packet information to file.
void write_ipv4(IPv4 ipv4_packet) {

#pragma region // IPv4 Protocol Definitions

    #ifndef UDP
        #define UDP 0x11
    #endif

    #ifndef TCP
        #define TCP 0x6
    #endif  

#pragma endregion  

    if(ipv4_writer.is_open()){
        
        ipv4_writer << "BEGIN_PACKET\n";
        write_mac_address(ipv4_packet, ipv4_writer);

        ipv4_writer << "SOURCE_IP: " << ipv4_packet.source_ip << "\n"
                    << "DEST_IP: " << ipv4_packet.dest_ip << "\n";
                    
        switch (ipv4_packet.ip_protocol) {
            case UDP:
                ipv4_writer << "IP_PROTO: UDP\n";
                break;
            case TCP:
                ipv4_writer << "IP_PROTO: TCP\n";
                break;
            default:
                ipv4_writer << "IP_PROTO_UNRECOGNIZED: " << ipv4_packet.ip_protocol << "\n";
                break;
        }
        
        ipv4_writer << "END_PACKET\n";
        ipv4_writer.flush(); // needed the flush.. data was not being written

    } else {

        std::cerr << "The log file could not be opened!";
    }
    
}

// Write the relevant IPv6 packet information to a file
void write_ipv6(IPv6 ipv6_packet){

    if (ipv6_writer.is_open()) {

        ipv6_writer << "BEGIN_PACKET\n";
        write_mac_address(ipv6_packet, ipv6_writer);

        ipv6_writer << "SOURCE_IP: " << ipv6_packet.source_addr << "\n"
                    << "DEST_IP: " << ipv6_packet.dest_addr << "\n";

        ipv6_writer << "END_PACKET\n";
        ipv6_writer.flush();

    } else {

        std::cerr << "The log file could not be opened!";
    }

}

// Check the Ethernet Protocol of the received packet.
// Analyze each type of packet separately. This will be fun later...
void ether_switch(unsigned char * buf) {

#pragma region // Ethernet Protocol Definitions

    #ifndef ATT_P
        #define ATT_P 0x7373
    #endif

    #ifndef IPv4_P
        #define IPv4_P 0x0800
    #endif

    #ifndef IPv6_P
        #define IPv6_P 0x86dd
    #endif

    #ifndef ARP_P
        #define ARP_P 0x0806
    #endif

#pragma endregion

    struct ethhdr *eth = (struct ethhdr *)(buf);
    
    switch(ntohs(eth->h_proto)) {

        case ATT_P: {

            //TODO Att attpacket = Att(buf);
        }
        break;

        case IPv4_P: {

            IPv4 ipv4packet = IPv4(buf);
            write_ipv4(ipv4packet);
        }
        break;

        case IPv6_P: {

            IPv6 ipv6packet = IPv6(buf);
            write_ipv6(ipv6packet);
        }  
        break;

        case ARP_P: {

            //TODO Arp arppacket = Arp(buf);
        }
        break;

        default:
            std::cout << "Unidentified Ethernet protocol encountered! " << std::hex << ntohs(eth->h_proto) << "\n";

    }
}

// Open a raw socket and begin listening for packets.
// NOTE: This currently reads 200 packets and stops listening.
int main() {

    printf("Listening for packets...\n");
    
    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL)); // raw socket

    if (sock<0) { // An error occured while creating the socket
        printf("Socket creation failed with error %d", errno);
    }

    unsigned char buf[1024];

    struct sockaddr saddr;
    int s_addr_len = sizeof(saddr);

    bool listening = true;
    int packet_num = 0;

    while (listening) {

        packet_num += 1;
        int read = recvfrom(sock, buf, 1024, 0, &saddr, (socklen_t *)&s_addr_len); // read bytes on the wire

        if (read<0) { // Something didn't work...
            printf("Call to recvfrom failed with error %d\n", errno);
        }
        
        ether_switch(buf);

        if (packet_num%25 == 0) {
            printf("%d packets read...\n", packet_num);
        }

        if (packet_num>200) {
            listening = false;
            return 0;
            //cleanup_writers(); // Close all of the file streams
        }
    }
}
