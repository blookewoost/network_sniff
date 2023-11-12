import os
import json

from Packet import Packet

class Loader:
    def __init__(self, _filepath):
        self.load(_filepath)
        self.collect_entries()
        
    def load(self, file):
        if os.path.exists(file):
            with open(file) as data:
                self.raw = json.loads(data.read())
        else:
            raise Exception("The filepath: {} does not exist.".format(file))
        
    def collect_entries(self):
        self.packets = []
        for entry in self.raw:
            self.packets.append(Packet(entry))

        self.total_packets = len(self.packets)
        ipv6_packet_num = 0
        ipv4_packet_num = 0
            
        for packet in self.packets:
            if int(packet.eth_proto, 16) == 34525:
                ipv6_packet_num += 1
            elif int(packet.eth_proto, 16) == 2048:
                ipv4_packet_num += 1

        print("A total of {} ipv6 packets were analyzed, making up {} percent of the packet capture".format(ipv6_packet_num, (ipv6_packet_num/self.total_packets)*100))
        print("A total of {} ipv4 packets were analyzed, making up {} percent of the packet capture".format(ipv4_packet_num, (ipv4_packet_num/self.total_packets)*100))

        



