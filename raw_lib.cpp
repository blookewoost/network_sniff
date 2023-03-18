#include<stdio.h>
#include<linux/if_ether.h>

void print_eth_details(ethhdr *eth) {
    printf("Source MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
        eth->h_source[0],
        eth->h_source[1],
        eth->h_source[2],
        eth->h_source[3],
        eth->h_source[4],
        eth->h_source[5]);

    printf("Destination MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
        eth->h_dest[0],
        eth->h_dest[1],
        eth->h_dest[2],
        eth->h_dest[3],
        eth->h_dest[4],
        eth->h_dest[5]);
}
