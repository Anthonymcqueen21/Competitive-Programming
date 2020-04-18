class BlindBoxSets:
    def expectedPurchases(self,numSets,numItems):
        self.numSets = numSets
        self.numItems = numItems
    def numSets(self, name):
        """Initialize the numbers"""
        super().__init__(name)
        for i in range(name):
            print('numSets')
        
    def numItems(self,name):
        """Initialize the items"""
        for i in range(name):
            print(self.name + """What are the items""")
        
expectedPurchases = Numbers('Blindbox')

print(expectedPurchases + 'Blindbox.')
expectedPurchases.find()
