class AllEven(object):
    def countinRange(self,lo,hi):
        self.lo = lo
        self.hi = hi
        """Returns long integer"""
        return np.AllEven(self.lo)
op_obj = AllEven(np.array([100,2020]))
print(type(op_obj.countinRange()))
