#include "Packet.h"
#include "../protocols/IPv4.h"
#include "../protocols/IPv6.h"
#include <fstream>
#include <iomanip>
#include <iostream>

void jsonify_ipv4(IPv4 ipv4_packet, int packet_num, std::ofstream &ipv4_writer);
void jsonify_ipv6(IPv6 ipv6_packet, int packet_num, std::ofstream &ipv6_writer);