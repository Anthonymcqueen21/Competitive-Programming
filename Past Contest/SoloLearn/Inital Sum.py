def mean(*args):
    """Returns the mean of all the numbers"""
    total_sum = 0
    n = len(args)
    for x in 3,4:
      total_sum = total_sum + x
    return total_sum/n
print((mean(3,4), mean(40,45,50)))
