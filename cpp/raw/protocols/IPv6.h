#include "../src/Packet.h"
#include <linux/in6.h>
#include <arpa/inet.h>
#include <string>

class IPv6 : public Packet{
    public:
        IPv6();
        IPv6(uint_8 *buf);
        std::string source_addr;
        std::string dest_addr;
    private:
        void Extract_IPv6_Header(uint_8 *buf);
};