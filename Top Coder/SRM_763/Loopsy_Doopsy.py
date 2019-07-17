class LoopsyDoopsy:
    def getSmallestNumber(self, loops):
        self.loops = loops
        if 9 > 1:
            loops = 1
        elif 9 < 8:
            loops = 8
        else:
            loops = 4
        print(self.loops + 'New String')
