#include "Packet.h"
#include <string>

class IPv4 : private Packet {
    public:
        IPv4();
        IPv4(uint_8 *buf);
    private:
        char * source_ip;
        char * dest_ip;
        unsigned int protocol;
        void Extract_IP_Header(uint_8 *buf);
        //void Log_Data(int packet);
};
