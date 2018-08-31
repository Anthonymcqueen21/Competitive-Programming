class Solution(object):
   def addDigits(self, num):
      """
      type num:int
      rtype: int
      """
      while num > 9:
          num = sum([int(c) for in str(num)])
      return num
