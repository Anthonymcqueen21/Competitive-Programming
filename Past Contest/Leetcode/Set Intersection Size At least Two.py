class Solution(object):
    def intersectionSizeTwo(self, intervals):
       """
       :type intervals: List([List[int]]
       :rtype: int
       """
       intervals.sort(key=lambda x: x[1])
       intersection = []
       
       for start, end in intervals:
           
           if not intersection or start > intersection[-1]:
               intersection.append(end - 1)
               intersection.append(end)
           elif start > intersection[-2]:
               intersection.append(end)
               
    return len(intersection)
