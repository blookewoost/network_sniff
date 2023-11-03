#include "IPv6.h"
#include <linux/ipv6.h>
#include <linux/if_ether.h>
#include <string>
#include <arpa/inet.h>

IPv6::IPv6() {

}

IPv6::IPv6(uint_8 *buf) {

    IPv6::Get_Ethernet_Frame(buf);
    IPv6::Extract_IPv6_Header(buf);
}

void IPv6::Extract_IPv6_Header(uint_8 *buf) {

    struct ipv6hdr *ipv6 = (struct ipv6hdr*) (buf + sizeof(struct ethhdr));

    struct in6_addr source_addr = ipv6->saddr;
    struct in6_addr dest_addr = ipv6->daddr;

    char saddr[INET6_ADDRSTRLEN];
    char daddr[INET6_ADDRSTRLEN];

    // Implicit cast to std::string
    IPv6::source_addr = inet_ntop(AF_INET6, &source_addr, saddr, INET6_ADDRSTRLEN);
    IPv6::dest_addr = inet_ntop(AF_INET6, &dest_addr, daddr, INET6_ADDRSTRLEN);

}