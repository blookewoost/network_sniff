
'''
Represents a single EtherType as defined in the ieee-802 database
'''
class EtherType:
    def __init__(self, type_data: list):
        if len(type_data) != 6: # Something quite unexpected happened.
            print(len(type_data))
            raise Exception("Attempted to create an EtherType without the appropriate fields.")
        else:
            self.define(type_data)

    def define(self, type_data: list):
        self.get_decimal_ethertype(type_data[0])
        self.description = type_data[4]
        
    # If the first entry contains a '-', the decimal ethertype is actually a range of values.
    def get_decimal_ethertype(self, decimal_info):

        if '-' in decimal_info:
            values = decimal_info.split('-')
            self.decimal = range(int(values[0]), int(values[1]))
        else: #
            self.decimal = int(decimal_info)
