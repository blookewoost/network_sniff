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
#include <filesystem>
#include "lib.h"
#include "defines.h"

#define ipv4_file "../data/ipv4.json"
#define ipv6_file "../data/ipv6.json"

// Open an output file stream for writing json
std::ofstream initialize_writer(std::string filepath){

    std::ofstream writer = std::ofstream(filepath, std::fstream::app);
    if (writer.is_open()) {
        writer << "{";
        writer.flush();
    } else {
        std::cerr << "Failed to open a file stream for: " << filepath << "\n";
    }

    return writer;
    
}

// Close output file stream for writing json
void finalize_writer(std::ofstream& writer) {
    if (writer.is_open()) {
        writer << "}";
        writer.flush();
        writer.close();
    } else {
        std::cerr << "Filestream was unexpectedly closed prior to finalization!\n";
    }
}


// Open a raw socket and begin listening for packets.
int main(int argc, char* argv[]) {

    int num = 0;
    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL)); // raw socket

    if (sock<0) { // An error occured while creating the socket
        printf("Socket creation failed with error %d", errno);
        return -1;
    }

    unsigned char buf[1024];

    struct sockaddr saddr;
    int s_addr_len = sizeof(saddr);

    if (argc>1) {
        num = atoi(argv[1]);
        printf("Listening for %d packets. \n", num);
    } else {
        printf("Please supply the number of packets to listen for as an argument. \n");
        return -1;
    }

    // Get ready to log packets
    std::ofstream ipv4_writer = initialize_writer(ipv4_file);
    std::ofstream ipv6_writer = initialize_writer(ipv6_file);

    bool listening = true;
    
    unsigned int ipv4_packet_num = 0;
    unsigned int ipv6_packet_num = 0;
    unsigned int total_packets = 0;

    while (listening) {

        int read = recvfrom(sock, buf, 1024, 0, &saddr, (socklen_t *)&s_addr_len); // read bytes on the wire

        if (read<0) { // Something didn't work...
            printf("Call to recvfrom failed with error %d\n", errno);
            return -1;

        } else if (read>0) {

            total_packets += 1;
            struct ethhdr *eth = (struct ethhdr *)(buf);
            int proto = ntohs(eth->h_proto);

            switch(proto) {
                case IPv4_P:
                    ipv4_packet_num += 1;
                    jsonify_ipv4(buf, ipv4_packet_num, ipv4_writer);
                case IPv6_P:
                    ipv6_packet_num += 1;
                    jsonify_ipv6(buf, ipv6_packet_num, ipv6_writer);
            }

            if (total_packets%25 == 0) {
                printf("%d packets read...\n", total_packets);
            }

            if (total_packets>=num) {
                listening = false;
            }
        }
    }

    finalize_writer(ipv4_writer);
    finalize_writer(ipv6_writer);

    return 0;
}
