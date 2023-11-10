#include "lib.h"

// Write the source and destination MAC addresses to file.
void jsonify_mac(Packet packet, std::ofstream& writer, int packet_num){ // Need reference to the writer stream's buffer.

    int i;

    writer << "\t\t\"src_mac\":\"";

    for (i=1;i<=sizeof(packet.source_mac);i++) { // Write the source MAC address in hexadecimal, padded with zeroes
        writer << std::setfill('0') << std::setw(2) << std::hex << (int)packet.source_mac[i-1];
        if (i<(sizeof(packet.source_mac))) {
            writer << ":";
        }
    }

    writer << "\",\n\t\t\"dest_mac\":\"";

    for (i=1;i<=sizeof(packet.dest_mac);i++) { // Write the destination MAC address in hexadecimal, padded with zeroes
        writer << std::setfill('0') << std::setw(2) << std::hex << (int)packet.dest_mac[i-1];
        if (i<(sizeof(packet.dest_mac))) {
            writer << ":";
        }
    }

    writer << "\",\n";
}

void jsonify_ipv4(IPv4 ipv4_packet, unsigned int packet_num, std::ofstream &writer) {


    if(writer.is_open()){

        if (packet_num != 1) {
            writer << ",";
        }

        writer << "\n\t\""  << std::dec << packet_num << "\":{\n";

        jsonify_mac(ipv4_packet, writer, packet_num);        
        writer << "\t\t\"proto\":\"" << ipv4_packet.ip_protocol << "\",\n";
        writer << "\t\t\"src_ip\":\"" << ipv4_packet.source_ip << "\",\n";  
        writer << "\t\t\"dest_ip\":\"" << ipv4_packet.dest_ip << "\"\n";
        writer << "\t}\n";

        writer.flush();

    } else {

        std::cerr << "The log file could not be opened!";
    }
    
}

// Write the relevant IPv6 packet information to a file
void jsonify_ipv6(IPv6 ipv6_packet, unsigned int packet_num, std::ofstream &writer){

    if (writer.is_open()) {

        if (packet_num != 1) {
            writer << ",";
        }

        writer << "\n\t\""  << packet_num << "\":{\n";

        jsonify_mac(ipv6_packet, writer, packet_num);

        writer << "\t\t\"src_ip\":\"" << ipv6_packet.source_addr << "\",\n";  
        writer << "\t\t\"dest_ip\":\"" << ipv6_packet.dest_addr << "\"\n";
        writer << "\t}\n";

        writer.flush();

    } else {

        std::cerr << "The log file could not be opened!";
    }

}