import os
import csv
from .EtherType import EtherType
'''
Container for the ieee-802 database entries
'''
class EtherTypes:
    def __init__(self, database):
        if os.path.exists(database):
            with open(database) as db:
                self.reader = csv.reader(db)
                self.parse()
        else:
            raise Exception("EtherTypes initialized with an invalid file: {}".format(database))
        
        self.build_type_dict()
        
    def parse(self):
        self.ether_types = []
        next(self.reader) # Skip the header line
        for row in self.reader:
            self.ether_types.append(EtherType(row))

    '''
    Build a dictionary of all the types and their description
    '''
    def build_type_dict(self):
        self.types = {}
        for type in self.ether_types:
            if isinstance(type.decimal, range):
                for value in type.decimal:
                    self.types.update({value: type.description})
            elif isinstance(type.decimal, int):
                self.types.update({type.decimal: type.description})
            else: # Something quite unexpected happened
                raise Exception("Encountered an invalid EtherType!")


