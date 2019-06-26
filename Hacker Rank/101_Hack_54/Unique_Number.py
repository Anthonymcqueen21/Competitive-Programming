# Write a program to see whether a given number is a ugly number.
# Ugly numbers are usually positive numbers whose prime factors
# Usually 1 number is usually treated as a ugly number

# Implementation
class Solution:
   def isUgly(self, num):
      if num == 0:
        return False
   for i in [2,3,5]:
      while num % i == 0:
          num /= i
   return num == 1
