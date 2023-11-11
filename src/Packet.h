#pragma once

typedef unsigned char uint_8;
typedef unsigned short uint_16;

// All of the Packet types inherit from this class, 
// since all of the packets will have an Ethernet Frame.
class Packet{
    public:
        Packet();
        Packet(uint_8 *buf);
        void Get_Ethernet_Frame(uint_8 *buf);
        uint_8 source_mac[6];
        uint_8 dest_mac[6];
        uint_16 eth_proto;
    private:
};
