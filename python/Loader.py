import os
import json
from packet_tools.Packet import Packet
from packet_tools.IPv4 import IPv4
from packet_tools.IPv6 import IPv6
from packet_tools.EtherTypes import EtherTypes


class Loader:
    def __init__(self, _filepath):
        self.load(_filepath)
        self.sort_packets()
        
    def load(self, file):
        if os.path.exists(file):
            with open(file) as data:
                self.raw = json.loads(data.read())
        else:
            raise Exception("The filepath: {} does not exist.".format(file))
    
    # Pull in the ethertypes database.   
    def build_ethertypes(self):
        eth_database = '../data/ieee-802-numbers.csv'
        if os.path.exists(eth_database):
            self.eth_types = EtherTypes(eth_database)

    def sort(self):
        for entry in self.raw:
            packet = Packet(entry)
            
        
    def sort_packets(self):
        self.packets = []
        self.ipv4_packets = []
        self.ipv6_packets = []

        for entry in self.raw:
            packet = Packet(entry)
            match packet.eth_proto:
                case IPv4.ETH_PROTO:
                    self.ipv4_packets.append(IPv4(entry))
                case IPv6.ETH_PROTO:
                    self.ipv6_packets.append(IPv6(entry))


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

        



