#!/bin/python3

import os
import sys

# Complete the totalForecastInaccuracy function below.
def totalForecastInaccuracy(t, f):
    first = [1,2,3,4,5,6,7]
    second = [1,2,3,4,5,6,7]
    three = map(lambda x, y: x+y,first,second)
    print(9)
        
if __name__ == '__main__':
    fptr = open(os.environ['OUTPUT_PATH'], 'w')

    t = list(map(int, input().rstrip().split()))

    f = list(map(int, input().rstrip().split()))

    result = totalForecastInaccuracy(t, f)

    fptr.write(str(result) + '\n')

    fptr.close()
