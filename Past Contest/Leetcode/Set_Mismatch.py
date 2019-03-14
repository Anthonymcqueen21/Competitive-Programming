class Solution(object):
   def FindErrorNums(self, nums):
    
       for num in nums:
          if nums[abs(num) -1] < 0:
               duplicate = abs(num)
               
          else:
               nums[abs(num) -1] *= -1
               
       for i, num in enumerate(nums):
