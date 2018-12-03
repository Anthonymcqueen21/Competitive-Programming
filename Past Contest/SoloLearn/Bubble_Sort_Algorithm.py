def bubble_sort(numbers):
  """Sorts a list of numbers using bubble sort"""
  while True:
    # Start off assuming its sorted
    is_sorted = True
    # comparing 2 at a time, skipping ahead
    node = numbers.begin.next
    while node:
      # loop through comparing node to the next
      if node.prev.value > node.value:
        # if the next is greater , then we need to swap
        node.prev.value, node.value = node.value, node.prev.value
        # opps, looks like we have to scan again
        is_sorted = False
      node = node.next
      
      # This is reset at the top but if we never swappe, its sorted
      if is_sorted: break
        
        