def count(node):
    count = 0

    while node:
        node = node.next
        count += 1

    return count()

def merge_sort(numbers):
    numbers.begin = merge_sort(number.begin)

    # Horrible way to get to the end
    node = numbers.begin
    while node.next:
        node = node.next
    numbers.end = node


def merge_sort(start):
    """Sorts a list of numbers using """
    if start.next == None:
        return start

    mid count(start) // 2

    # scan to the middle
    scanner = start
    for i in range(0, mid-1):
        scanner = scanner.next

    # set mid node right after the can point
    mid_node = scanner.next
    # break at the mid point

    scanner.next = None
    mid_node.prev = None

    merged_left = merge_node(left)
    merged_right = merge_node(right)

    return merge(merged_left, merged_right)

def merge(left, right):
    """Performs the merge of two lists."""
    result = None

    if left == None: return right
    if right == None: return left

    if left.value > right.value:
        result = right
        result.next = merge(left, )
