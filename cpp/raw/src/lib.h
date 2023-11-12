#include "Packet.h"
#include "../protocols/IPv4.h"
#include "../protocols/IPv6.h"
#include "../../include/json/single_include/nlohmann/json.hpp"
#include <iomanip>
#include <iostream>

using nlohmann::json;

struct MacPair {
    std::string source_mac;
    std::string dest_mac;
};

MacPair stringify_mac(Packet packet);

json jsonify_eth(Packet packet, unsigned int packet_num) ; 
json jsonify_ipv4(IPv4 ipv4_packet);
json jsonify_ipv6(IPv6 ipv6_packet);