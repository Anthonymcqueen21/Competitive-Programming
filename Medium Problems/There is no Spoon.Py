import sys
from collections import OrderedDict, deque
class Node():
    """
    Basic structure for nodes/islands.
    Contains the information about position and neighbors
    and very few logic.
    """
    right = None
    left = None
    down = None
    up = None
    def __init__(self, pos, amount, number):
        self.pos = pos
        self.amount = amount
        self.x = pos[0]
        self.y = pos[1]
        self.number = number

    def after(self):
        return(int(self.right is not None) + int(self.down is not None))

    def before(self):
        return(int(self.left is not None) + int(self.up is not None))

    def __str__(self):
        return('Node at {1.pos}'.format(self))

# Read the input.
data = []
width = int(input())  # the number of cells on the X axis
height = int(input())  # the number of cells on the Y axis
for i in range(height):
    line = input()  # width characters, each either a number or a '.'
    data.append(list(line))

nodes = OrderedDict()
nodenum = 0
# Generate `Node` object for the nodes.
for y in range(height):
    for x in range(width):
        if data[y][x].isnumeric():
            nodes[(x, y)] = Node((x, y), int(data[y][x]), nodenum)
            nodenum += 1

# The total number of the nodes.
N = len(nodes)

# Find all the neighbors for each node.
for (x, y), node in nodes.items():
    for xx in range(x+1, width):
        if (xx, y) in nodes:
            node.right = nodes[(xx, y)]
            nodes[(xx, y)].left = node
            break
    for yy in range(y+1, height):
        if (x, yy) in nodes:
            node.down = nodes[(x, yy)]
            nodes[(x, yy)].up = node
            break

for node in nodes.values():
    print(
        node.x, node.y,
        node.right.x if node.right else -1,
        node.right.y if node.right else -1,
        node.down.x if node.down else -1,
        node.down.y if node.down else -1,
    )
