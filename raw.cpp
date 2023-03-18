#include "raw_lib.h"
#include "IPv4.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sys/socket.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <arpa/inet.h>
#include <errno.h>
#include <cstdlib>
#include <iomanip>

#define ATT_PROTO 0x7373
#define IPv4_PROTO 0x800
#define IPv6_PROTO 0x86dd

std::ofstream ipv4_writer("ipv4.txt", std::fstream::app);

void write_ipv4(IPv4 ipv4_packet, int packet_num) {

    if(ipv4_writer.is_open()){
        ipv4_writer << "BEGIN_PACKET:" << packet_num << "\n";
        ipv4_writer.flush(); // needed the flush.. data was not being written
    } else {
        std::cerr << "The log file could not be opened!";
    }
    
}

void breakdown_ipv4(unsigned char * buf) {

    struct iphdr *ip = (struct iphdr*) (buf + sizeof(struct ethhdr));

    struct sockaddr_in source; // Store IP addresses in sockaddr, so that we can use inet functions to extract a readable IP.
    source.sin_addr.s_addr = ip->saddr;

    struct sockaddr_in dest;
    dest.sin_addr.s_addr = ip->daddr;

    printf("Source IP Adress: %s\n", inet_ntoa(source.sin_addr));
    printf("Destination IP Adress: %s\n", inet_ntoa(dest.sin_addr));
    printf("Protocol: %d\n", (unsigned int)ip->protocol);

    //breakdown the IPv4 header.. https://en.wikipedia.org/wiki/Internet_Protocol_version_4#Header

}

// Check the Ethernet Protocol of the received packet.
// Analyze each type of packet separately. This will be fun later...
void ether_switch(unsigned char * buf, int packet_num) {

    struct ethhdr *eth = (struct ethhdr *)(buf);
    
    switch(ntohs(eth->h_proto)) {

        case ATT_PROTO: {

            std::cout << "Found a bullshit AT&T packet.. let's skip these for now.\n";
            //TODO breakdown_att(buf);
        }
        break;

        case IPv4_PROTO: {

            std::cout << "Found an IPv4 packet! Let's break this down.\n";
            IPv4 ipv4packet = IPv4(buf);
            write_ipv4(ipv4packet, packet_num);
        }
        break;

        case IPv6_PROTO: {

            std::cout << "Found an IPv6 packet! Let's break this down.\n";
            // TODO breakdown_ipv6(buf);
        }  
        break;

        default:
            std::cout << "Unidentified Ethernet protocol encountered! " << std::hex << ntohs(eth->h_proto) << "\n";

    }
}

// Open a raw socket and begin listening for packets.
// NOTE: This currently reads one packet and stops listening.
int main() {
    
    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL)); // raw socket

    if (sock<0) { // An error occured while creating the socket
        printf("Socket creation failed with error %d", errno);
    }

    unsigned char buf[1024];

    struct sockaddr saddr;
    int s_addr_len = sizeof(saddr);

    //initialize_writers(); // Get ready to log data.

    bool listening = true;
    int packet_num = 0;

    while (listening) {

        packet_num += 1;
        int read = recvfrom(sock, buf, 1024, 0, &saddr, (socklen_t *)&s_addr_len); // read bytes on the wire

        if (read<0) { // Something didn't work...
            printf("Call to recvfrom failed with error %d", errno);
        }
        
        ether_switch(buf, packet_num);

        if (packet_num>10000) {
            listening = false;
            //cleanup_writers(); // Close all of the file streams
        }
    }
}
