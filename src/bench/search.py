import subprocess
import os

#files = ['1mb', '2mb', '3mb', '4mb', '6mb', '7mb', '8mb', '9mb']
files = [('%smb' % d) for d in list(range(1,5)) + list(range(6,10)) ]
repetitions = 3

def getSize(fileName):
	info = os.stat(fileName)
	size = info.st_size
	
	return size / 1000000.0

def executeAndGetTime(line, preventPrinting = False):
	cmd = '/bin/bash -c \"time %s "' % (line)
	if not preventPrinting:
		print cmd
	p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	out, err = p.communicate()
	
	lines = err.split('\n')
	t = -1
	for line in lines:
		if 'real' in line:
			times = line.split()
			
			if not preventPrinting:
				print out
				print err
			m = float(times[1].split('m')[0])
			s = float(times[1].split('m')[1].split('s')[0])
			t = m * 60 + s
	
	if(t == -1):
		print err
		print out
	return t



headers = ['FileSize', 'Setup', 'Search']
saFile = open('sa.results', 'a')
stFile = open('st.results', 'a')
grepFile = open('grep.results', 'a')

#saFile.write((3 * '%20s') % tuple(headers))
#saFile.write('\n')
#stFile.write((3 * '%20s') % tuple(headers))
#stFile.write('\n')
#grepFile.write((3 * '%20s') % tuple(headers))
#grepFile.write('\n')

for f in files:
	saResults = 2 * [0]
	stResults = 2 * [0]
	grepResults = 2 * [0]
	
#	for i in range(repetitions):
#	with open(f + '.pat', 'r') as patFile:
#		cnt = 0
#		for line in patFile:
#			if cnt % 1000 == 0:
#				print cnt
#			cnt += 1
#			line = line[:-1]
#			grepResults[1] += executeAndGetTime('grep -F -e \\"%s\\" %s -c' % (line, f), True ) 
		
	executeAndGetTime('./ipmt index %s --indextype suffixtree' % f)
	for i in range(repetitions):
		stResults[0] += executeAndGetTime('./ipmt search -p %s %s -c -i 2 ' % (f + '.pat', f + '.idx', ))
		stResults[1] += executeAndGetTime('./ipmt search -p %s %s -c > st.%s' % (f + '.pat', f + '.idx', f))
	
	executeAndGetTime('./ipmt index %s --indextype suffixarray' % f)
	for i in range(repetitions):
		saResults[0] += executeAndGetTime('./ipmt search -p %s %s -c -i 2 ' % (f + '.pat', f + '.idx', ))
		saResults[1] += executeAndGetTime('./ipmt search -p %s %s -c > sa.%s' % (f + '.pat', f + '.idx', f))


	stLine = [getSize(f), stResults[0]/repetitions, (stResults[1]-stResults[0])/repetitions]
	stFile.write( (3 * '%20f') % tuple(stLine))
	stFile.write('\n')
	stFile.flush()

	saLine = [getSize(f), saResults[0]/repetitions, (saResults[1]-saResults[0])/repetitions]
	saFile.write( (3 * '%20f') % tuple(saLine))
	saFile.write('\n')
	saFile.flush()

	grepLine = [getSize(f), 0, grepResults[1] ] # ATENCAO, NAO TIRA A MEDIA
	grepFile.write( (3 * '%20f') % tuple(grepLine))
	grepFile.write('\n')
	grepFile.flush()

saFile.close()
stFile.close()
grepFile.close()

