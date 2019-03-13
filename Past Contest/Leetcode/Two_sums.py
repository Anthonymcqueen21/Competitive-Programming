class Solution(object):
     def twoSum(self, nums, target):
         num_to_index = {}
         for i, num in enumerate(nums):
             if target - num in num_to_index:
                return [i, num_to_index[target - num]]
             num_to_index[num] = i
return [] # no sums
