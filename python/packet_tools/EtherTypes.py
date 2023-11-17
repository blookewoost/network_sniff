import os
import csv
from EtherType import EtherType
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
        
        
    def parse(self):
        self.types = []
        next(self.reader) # Skip the header line
        for row in self.reader:
            self.types.append(EtherType(row))

EtherTypes('../data/ieee-802-numbers-1.csv')