import os
import json
from packet_tools.Packet import Packet
from packet_tools.IPv4 import IPv4
from packet_tools.IPv6 import IPv6
from packet_tools.EtherTypes import EtherTypes


class Loader:
    def __init__(self, _filepath):
        self.load(_filepath)
        self.build_ethertypes()
        self.identify_packets()
        self.basic_report()
        
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
            eth_types = EtherTypes(eth_database)
            self.types = eth_types.types

    def identify_packets(self):
        self.no_id = []
        self.proto_counts = {}

        for entry in self.raw:
            packet = Packet(entry)
            proto = int(packet.eth_proto, 16)
            if proto in self.types.keys():
                packet.set_ether_description(self.types[proto])
                if not(proto in self.proto_counts.keys()): # Is there a better way to do this? Seems a little hacky.
                    self.proto_counts[proto] = 1
                else:
                    self.proto_counts[proto] += 1
            else:
                self.no_id.append(packet)


    def basic_report(self):
        total_packets = 0
        total_packets += len(self.no_id)
        for proto in self.proto_counts:
            total_packets += self.proto_counts[proto]

        print("A total of {} packets were parsed.".format(total_packets))

        percent_no_id = (len(self.no_id)/total_packets)*100
        print("{} percent of the packet capture was an unidentified protocol.".format(percent_no_id))
        for proto in self.proto_counts:
            description = self.types[proto]
            percent = (self.proto_counts[proto]/total_packets)*100
            print("{} percent of the packet capture was of type {}".format(percent, description))

        
        


        



