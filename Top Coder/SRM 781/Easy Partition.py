class EasyPartition():
    def getPartition(self,N):
        self.N = N
        if N == []:
            return [[]]
        N=subs(N[1:])
        return N+[[1[0]]+y for y in x]
