from Packet import Packet

class IPv4(Packet):
    ETH_PROTO = 2048

    def __init__(self, packet:dict):
        super().__init__(packet) # Explicitly call the constructor on the base class.
        self.parse(packet)

    def parse(self, packet):
        print("")

