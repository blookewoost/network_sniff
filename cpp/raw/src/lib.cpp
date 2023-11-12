#include "lib.h"
#include <sstream>

// Accept a packet and create a json structure for it.
json jsonify_eth(Packet packet, unsigned int packet_num) {

    MacPair macs = stringify_mac(packet);

    std::stringstream proto_write;
    proto_write << std::setfill('0') << std::setw(4) << std::hex << ntohs(packet.eth_proto);
    std::string proto = proto_write.str();

    json j;
    j["idx"] = packet_num;
    j["eth"] = {
        {"proto", proto},
        {"source_mac", macs.source_mac},
        {"dest_mac", macs.dest_mac}
    };
    
    return j;
}

json jsonify_ipv4(IPv4 ipv4_packet) {

    json j;

    j["ipv4"] = {
        {"proto", ipv4_packet.ip_protocol},
        {"source_addr", ipv4_packet.source_ip},
        {"dest_addr", ipv4_packet.dest_ip}
    };

    return j;
}

json jsonify_ipv6(IPv6 ipv6_packet) {

    json j;

    j["ipv6"] = {
        {"source_addr",ipv6_packet.source_addr},
        {"dest_addr", ipv6_packet.dest_addr}
    };

    return j;
}

MacPair stringify_mac(Packet packet) {

    std::stringstream sstream;
    for (int i=1;i<=sizeof(packet.source_mac);i++) {
        sstream << std::setfill('0') << std::setw(2) << std::hex << (int)packet.source_mac[i-1];
        if (i<(sizeof(packet.source_mac))) {
            sstream << ":";
        }
    }

    std::string source_mac = sstream.str();

    //sstream.clear(); This clearly doesn't do what I expected
    sstream.str(""); // Reset the buffer

    for (int i=1;i<=sizeof(packet.dest_mac); i++) {
        sstream << std::setfill('0') << std::setw(2) << std::hex << (int)packet.dest_mac[i-1];
        if (i<(sizeof(packet.dest_mac))) {
            sstream << ":";
        }
    }

    std::string dest_mac = sstream.str();

    return MacPair{source_mac, dest_mac};
}
