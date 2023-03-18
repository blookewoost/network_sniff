#include "IPv4.h"
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <arpa/inet.h>
#include <filesystem>
#include <fstream>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <iterator>

#define UDP 17
#define TCP 6

using path = std::filesystem::path;

const path data_file{"/data/ipv4.txt"};
const path debug_file{"/data/ipv4_dbg.txt"};

IPv4::IPv4(){

}

IPv4::IPv4(uint_8 *buf) {

    IPv4::Get_Ethernet_Frame(buf);
    IPv4::Extract_IP_Header(buf);
    //IPv4::Log_Data(packet);

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

    //switch ((unsigned int)ip->protocol) {
    //    case UDP:
    //        printf("UDP Packet found!");
    //        IPv4::protocol = "UDP";
    //        break;
    //    case TCP:
    //        printf("TCP Packet found!");
    //        IPv4::protocol = "TCP";
    //        break;
    //    default:
    //        std::cout << "Unidentified IP protocol encountered! " << (unsigned int)(ip->protocol) << "\n";
    //}
}

//void IPv4::Log_Data(int packet) {
//
//
//    std::ofstream datalogger;
//
//    //const int protocols[] = {UDP, TCP};
//    
//    if (IPv4::protocol != "UDP" && IPv4::protocol != "TCP"){
//        datalogger.open(debug_file, std::fstream::app);
//        if (datalogger) {
//            datalogger << "PACKET" << packet << "/n";
//            datalogger.close();
//                       //<< "Source MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n", IPv4::source_mac[1];
//            }
//    } else {
//        datalogger.open(data_file, std::fstream::app);
//        if (datalogger) {
//            datalogger << "PACKET" << packet << "/n";
//            datalogger.close();
//        } else {
//            std::cerr << "Log file could not be opened!/n";
//        }
//    }
//
//    //if (!known) {
//    //    datalogger.open(debug_file, std::fstream::app);
//    //    if (datalogger) {
//    //        datalogger << "PACKET" << packet << "/n"
//    //                   << "Source MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n", IPv4::source_mac[1];
//    //    }
//    //}
//
//}
