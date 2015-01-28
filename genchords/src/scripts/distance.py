# /bin/python

import sys

oldline=0.0
datei = open(sys.argv[1], 'r');
line = datei.readline()
repeat = 0
sum = 0
count = 0
averagediff = 0.0
while (line):
	line = line.replace(',','.')
	diff = (float(line)-oldline)
	averagediff += diff
	count += 1
	# print diff
	oldline=float(line)
	line = datei.readline()
print averagediff/count
