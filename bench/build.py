import subprocess
import os

#textFiles = ['1mb', '2mb', '3mb', '5mb', '6mb', '7mb', '8mb', '9mb', '10mb', '20mb', '30mb', '40mb', '50mb']
textFiles = ['90mb']
repetitions = 1
numberOfFiles = 1

def getSize(fileName):
	info = os.stat(fileName)
	size = info.st_size
	
	return size / 1000000.0

def executeAndGetTime(line):
	cmd = '/bin/bash -c \"time %s "' % (line)
	print cmd
	p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	out, err = p.communicate()
	
	lines = err.split('\n')
	t = -1
	for line in lines:
		if 'real' in line:
			times = line.split()
			print out
			print err
			m = float(times[1].split('m')[0])
			s = float(times[1].split('m')[1].split('s')[0])
			t = m * 60 + s
	
	if(t == -1):
		print err
		print out
	return t

timeFile = open('time.results', 'a')
sizeFile = open('size.results', 'a')
headers = ['FileSize', 'Arvore de Sufixos', 'Array de Sufixos', 'Arvore de Sufixos 2']
#timeFile.write( (4 * '%20s') % tuple(headers))
#timeFile.write('\n')
#sizeFile.write( (3 * '%20s') % tuple(headers[:-1]))
#sizeFile.write('\n')

for aux in textFiles:
	timeResults = 3 * [0]
	sizeResults = 2 * [0]

	for f in range(numberOfFiles):
		text =  aux + '_' + str(f)
		cmdline = './ipmt index %s -i %d --indextype %s --compression none'
		
		executeAndGetTime(cmdline % (text, 1, 'suffixtree'))
		sizeResults[0] += getSize(text + '.idx')
		
		#executeAndGetTime(cmdline % (text, 1, 'suffixarray'))
		#sizeResults[1] += getSize(text + '.idx')

		originalFileSize = getSize(text)

		for r in range(repetitions):
			timeResults[0] += executeAndGetTime(cmdline % (text, 0, 'suffixtree'))
	#		timeResults[1] += executeAndGetTime(cmdline % (text, 0, 'suffixarray'))
			timeResults[2] += executeAndGetTime(cmdline % (text, 0, 'suffixtree2'))

	timeLine = [originalFileSize]

	for value in timeResults:
		timeLine += [value / float(repetitions*numberOfFiles)]
	
	timeFile.write( (4 * '%20.3lf') % tuple(timeLine))
	timeFile.write('\n')
	timeFile.flush();

	sizeLine = [originalFileSize]
	
	for value in sizeResults:
		sizeLine += [value / float(numberOfFiles)]
	sizeFile.write( (3 * '%20.3lf') % tuple(sizeLine))
	sizeFile.write('\n')
	sizeFile.flush()


timeFile.close();
sizeFile.close()	


