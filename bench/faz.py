import subprocess
import os

algo = 'lz78' 
index = 'suffixarray'
patternsFile = 'patterns'
textFiles = ['100k', '200k', '300k', '700k', '1mb', '2mb', '3mb', '5mb']

acc = open("acc_%s_%s"%(algo,index), "w")
wa = open("wa_%s_%s" % (algo, index), "w")
acc.write((6 * '%15s|' + '\n') % ('Text', 'Construcao', 'Compressao', 'Descompressao', 'Consultas', 'Aquivo de indice'))
wa.write((6 * '%15s|' + '\n') % ('Arquivo', 'Construcao', 'Compressao', 'Descompressao', 'Consultas', 'Arquivo de indice'))

def executeAndGetTime(line):
	cmd = '/bin/bash -c \"time ./a.out %s "' % (line)
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

def printResults(text, results):
	acc.write(('%15s|' * (1+len(results)) + '\n') % tuple([text] + map(str, results)))
	acc.flush()
	last = 0
	for i in range(len(results)-1):
		results[i] = results[i] - last
		last = results[i]
		if i == 1:
			last = 0
	wa.write(('%15s|' * (1+len(results)) + '\n') % tuple([text] + map(str, results)))
	wa.flush()
	
for text in textFiles:
	print 'Testando arquivo %s' % text
	results = []
	for i in range(2):
		line = 'index --compression %s --indextype %s -i %d %s' % (algo, index, i, text)
		results += [executeAndGetTime(line)]
	line = 'search -c -p %s %s -i 2' % (patternsFile, text+'.idx')
	results += [executeAndGetTime(line)] 
	line = 'search -c -p %s %s' % (patternsFile, text+'.idx')
	results += [executeAndGetTime(line)] 
	
	info = os.stat(text+'.idx')
	size = info.st_size
	if(size < 2**20):
		size = '%.0lfKb' % (float(size)/2**10)
	else:
		size = '%.0lfMb' % (float(size)/2**20)
	results += [size]

	printResults(text, results)

acc.close()
wa.close()
