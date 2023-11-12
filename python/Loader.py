import os
import json

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
        for entry in self.raw:
            print(entry)




