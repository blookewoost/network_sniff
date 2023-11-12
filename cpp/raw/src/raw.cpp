#include <sys/socket.h>
#include <linux/if_ether.h>
#include <fstream>

#include "lib.h"
#include "defines.h"

#define outfile "../../../data/out.json"

using nlohmann::json;

// Open an output file stream for writing json
std::ofstream initialize_writer(std::string filepath){

    std::ofstream writer = std::ofstream(filepath, std::fstream::app);
    if (!writer){
        // Something bad happened
        std::cerr << "Filestream could not be opened!\n";
    }

    return writer;
    
}

// Close output file stream for writing json
void finalize_writer(std::ofstream& writer) {
    if (writer.is_open()) {
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
    std::ofstream writer = initialize_writer(outfile);

    bool listening = true;
    unsigned int total_packets = 0;

    json data;

    while (listening) {

        int read = recvfrom(sock, buf, 1024, 0, &saddr, (socklen_t *)&s_addr_len); // read bytes on the wire

        if (read<0) { // Something didn't work...
            printf("Call to recvfrom failed with error %d\n", errno);
            return -1;

        } else if (read>0) {

            total_packets += 1;
            struct ethhdr *eth = (struct ethhdr *)(buf);
            int proto = ntohs(eth->h_proto);

            json packet_info = jsonify_eth(buf, total_packets);
            
            switch(proto) {
                case IPv4_P:
                    packet_info.update(jsonify_ipv4(buf));
                    break;
                case IPv6_P:
                    packet_info.update(jsonify_ipv6(buf));
                    break;
            }

            if (!packet_info.empty()){
                data += packet_info;
            }
            
            if (total_packets%25 == 0) {
                printf("%d packets read...\n", total_packets);
            }

            if (total_packets>=num) {
                listening = false;
            }
            packet_info.clear();
        }
    }

    writer << data.dump(2);
    finalize_writer(writer);

    return 0;
}
