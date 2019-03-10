from collections import Counter

class Solution(object):
    def permuteUnique(self, nums):
    
      freq = Counter(nums)
      permutations = []
      self.permute_helper(len(nums), [], freq, permutations)
      return permutations
      
    def permute_helper(self, to_add, partial, freq, permutations):
      if to_add == 0:
         permutations.append(partial)
         
      for item in freq:
         if freq[item] > 0:
              freq[item] -= 1
              self.permute_helper(to_add-1, partial + [item], freq, permutations)
              freq[item] += 1
              
    
    
return item
