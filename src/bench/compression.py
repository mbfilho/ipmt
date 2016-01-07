import subprocess
import os

#textFiles = ['1mb', '2mb', '3mb', '5mb', '6mb', '7mb', '9mb', '10mb', '15mb']
textFiles = ['20mb', '25mb', '30mb', '40mb', '50mb']
repetitions = 3
numberOfFiles = 3

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
ratioFile = open('ratio.results', 'a')
headers = ['FileSize', 'lz77_128_8', 'lz77_1024_16', 'lz77_4096_32', 'lzw_0', 'lzw_1', 'lzw_2', 'lz78_0', 'lz78_1', 'lz78_2', 'gzip']
#timeFile.write( (11 * '%13s') % tuple(headers))
#timeFile.write('\n')
#ratioFile.write( (11 * '%13s') % tuple(headers))
#ratioFile.write('\n')

for aux in textFiles:
	timeResults = 10 * [0]
	ratioResults = 10 * [0]

	for f in range(numberOfFiles):
		text = '../' + aux + '_' + str(f)
		cmdline = '../comp --mode c --algo %s --input "%s" --output "%s" --wb %d --wl %d --level %d'
		configs = [
			('lz77', text, text + '_lz77_128_8.cmp', 128, 8, -1),
			('lz77', text, text + '_lz77_1024_16.cmp', 1024, 16, -1),
			('lz77', text, text + '_lz77_4096_32.cmp', 4096, 32, -1),
			('lzw', text, text + '_lzw_0.cmp', -1, -1, 0),
			('lzw', text, text + '_lzw_1.cmp', -1, -1, 1),
			('lzw', text, text + '_lzw_2.cmp', -1, -1, 1),
			('lz78', text, text + '_lz78_0.cmp', -1, -1, 0),
			('lz78', text, text + '_lz78_1.cmp', -1, -1, 1),
			('lz78', text, text + '_lz78_2.cmp', -1, -1, 1),
			 ]

		originalFileSize = getSize(text)

		for r in range(repetitions):
			for i,c in enumerate(configs):
				timeResults[i] += executeAndGetTime(cmdline % c)
			timeResults[9] += executeAndGetTime('gzip -9 -c %s > %s' % (text, text + '_gzip.cmp'))
	
			for i,c in enumerate(configs):
				compressedFile = c[2] 
				ratioResults[i] += getSize(compressedFile)/originalFileSize 
			ratioResults[9] += getSize(text + '_gzip.cmp')/originalFileSize 


	timeLine = [originalFileSize]

	for value in timeResults:
		timeLine += [value / float(repetitions*numberOfFiles)]
	
	timeFile.write( (11 * '%13.3lf') % tuple(timeLine))
	timeFile.write('\n')
	timeFile.flush();

	ratioLine = [originalFileSize]
	
	for value in ratioResults:
		ratioLine += [value / (repetitions*numberOfFiles)]
	ratioFile.write( (11 * '%13.3lf') % tuple(ratioLine))
	ratioFile.write('\n')
	ratioFile.flush()


timeFile.close();
	


