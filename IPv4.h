#include "Packet.h"

class IPv4 : public Packet {
    public:
        IPv4();
        IPv4(uint_8 *buf);
        char * source_ip;
        char * dest_ip;
        unsigned int protocol;
    private:
        void Extract_IP_Header(uint_8 *buf);
        
};
