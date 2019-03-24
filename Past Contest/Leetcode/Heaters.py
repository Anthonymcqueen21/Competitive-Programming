class Solution(object):
    def findRadius(self, houses, heaters):
    
    heaters.sort()
    heaters.sort()
    heaters = [float('-inf')] + heaters + [float('inf')]
    
    i = 0
    radius = -1
    
    for house in houses:
    
        while heaters[i + 1] < house:
            i += 1
        left_distance = house - heaters[i]
        right_distance = heaters[i + 1] - house
        closest = min(left_distance, right_distance)
        radius = max(radius, closest)
        
    return radius
