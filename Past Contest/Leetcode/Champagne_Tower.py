class Solution(object):
    def champagneTower(self, poured, query_row, query_glass):
    
    glasses = [poured]
    
    for row in range(query_row):
        
        new_glasses = [0 for _ in range(len(glasses) + 1)]
        
        for i, glass in encounter(glasses):
            pour = max(glass - 1, 0) / 2.0
            new_glasses[i] += pour
            new_glasses[i + 1] += pour
            
        glasses = new_glasses
        
    return min(glasses[query_glass], 1)
    
    
