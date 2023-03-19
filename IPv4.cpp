#include "IPv4.h"
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <arpa/inet.h>

IPv4::IPv4(){

}

IPv4::IPv4(uint_8 *buf) {

    IPv4::Get_Ethernet_Frame(buf);
    IPv4::Extract_IP_Header(buf);
}


void IPv4::Extract_IP_Header(uint_8 *buf) {

    struct iphdr *ip = (struct iphdr*) (buf + sizeof(struct ethhdr));

    struct sockaddr_in source;
    source.sin_addr.s_addr = ip->saddr;
    struct sockaddr_in dest;
    dest.sin_addr.s_addr = ip->saddr;

    IPv4::source_ip = inet_ntoa(source.sin_addr);
    IPv4::dest_ip = inet_ntoa(dest.sin_addr);
    IPv4::protocol = (unsigned int)ip->protocol;

}

