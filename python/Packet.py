import json

class Packet:
    def __init__(self, packet: dict):
        self.parse(packet)

    def parse(self, packet):
        if 'idx' in packet.keys():
            self.packet_id = packet['idx']
        else:
            self.handle_no_packet_id()
            raise Exception("A malformed json 'packet' object was parsed!")
        
        if 'eth' in packet.keys():
            self.source_mac = packet['eth']['source_mac']
            self.dest_mac = packet['eth']['dest_mac']
            self.eth_proto = packet['eth']['proto']
        
    def handle_no_packet_id(self):
        # TODO: Implement decent error handling.
        pass