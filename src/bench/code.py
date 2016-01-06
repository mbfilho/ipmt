from math import log

def sizeInBits(t):
	if t == 0:
		return 1
	return int(log(t, 2)) + 1

def GP(t):
	if t <= 1:
		return 1
	return t + GP(sizeInBits(t-2))

def G(t):
	return 1 + GP(t+1) 

def M(t):
	return t + 1 + 2 * sizeInBits(t) 

C = [0] + [G(i) for i in range(1, 33)]
print ('int C[] = {' + ('%d,' * (len(C)-1)) + '%d' + '};') % tuple(C)

for i in range(1, 33):
	print 'Size %d: Code: %d Meu %d' % (i, G(i), M(i))



