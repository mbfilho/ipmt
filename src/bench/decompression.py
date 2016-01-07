import subprocess
import os

#textFiles = ['1mb', '2mb', '3mb', '5mb', '6mb', '7mb', '9mb', '10mb', '15mb']
textFiles = ['20mb', '25mb', '30mb', '40mb', '50mb']
repetitions = 3
numberOfFiles = 3

def getSize(fileName):
	info = os.stat(fileName)
	size = info.st_size
	
	return size

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
headers = ['FileSize', 'lz77_128_8', 'lz77_1024_16', 'lz77_4096_32', 'lzw_0', 'lzw_1', 'lzw_2', 'lz78_0', 'lz78_1', 'lz78_2', 'gzip']
#timeFile.write( (11 * '%13s') % tuple(headers))
#timeFile.write('\n')

confere = open('confere.sh', 'w')
for aux in textFiles:
	timeResults = 10 * [0]
	for f in range(numberOfFiles):
		text = '../' + aux + '_' + str(f)
		cmdline = '../comp --mode d --algo %s --input "%s" --output "%s" --originalFileSize %d'
		originalFileSize = getSize(text) 
		configs = [
		 ('lz77', text + '_lz77_128_8.cmp', text + '_lz77_128_8.dmp',  originalFileSize),
		 ('lz77', text + '_lz77_1024_16.cmp', text + '_lz77_1024_16.dmp',  originalFileSize),
		 ('lz77', text + '_lz77_4096_32.cmp', text + '_lz77_4096_32.dmp',  originalFileSize),
		 ('lzw', text + '_lzw_0.cmp', text + '_lzw_0.dmp',  originalFileSize),
		 ('lzw', text + '_lzw_1.cmp', text + '_lzw_1.dmp',  originalFileSize),
		 ('lzw', text + '_lzw_2.cmp', text + '_lzw_2.dmp',  originalFileSize),
		 ('lz78', text + '_lz78_0.cmp', text + '_lz78_0.dmp',  originalFileSize),
		 ('lz78', text + '_lz78_1.cmp', text + '_lz78_1.dmp',  originalFileSize),
		 ('lz78', text + '_lz78_2.cmp', text + '_lz78_2.dmp',  originalFileSize),
		 ]
	
		for r in range(repetitions):
			for i,c in enumerate(configs):
				confere.write('cmp %s %s\n' % (text, c[2]))
				timeResults[i] += executeAndGetTime(cmdline % c)
			timeResults[9] += executeAndGetTime('gzip -c -d %s > %s' % (text + '_gzip.cmp', text + '_gzip.dmp'))

	timeLine = [originalFileSize/1000000.0]
	
	for value in timeResults:
		timeLine += [value / float(numberOfFiles * repetitions)]
	timeFile.write( (11 * '%13.3lf') % tuple(timeLine))
	timeFile.write('\n')
	timeFile.flush();

confere.close()
timeFile.close();
	


