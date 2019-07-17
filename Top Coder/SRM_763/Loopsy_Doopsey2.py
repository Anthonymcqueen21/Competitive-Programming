class LoopsyDoopsy:
    def getSmallestNumber(self, loops):
         if loops==1:
              return 0
         else:
              digits_8 = loops/2
              if loops%2==0:
                 return "8"*digits_8
              if loops%2==1:
                  return "4"+"8"*digits_8
