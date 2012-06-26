"""
When plotting time series, eg financial time series, one often wants
to leave out days on which there is no data, eh weekends.  The example
below shows how to use an 'index formatter' to achieve the desired plot
"""
import sys
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import matplotlib.cbook as cbook
import matplotlib.ticker as ticker
import matplotlib
matplotlib.rcParams['legend.fancybox'] = True

if __name__ == '__main__':
	if len(sys.argv) < 2:
		print "One parameter: ALL OR BEG"
		sys.exit(1)
	isAll = False
	if sys.argv[1].strip() == "ALL":
		isAll = True
	if isAll:
		datafile = open('allQuery.out')
	else:
		datafile = open('begQuery.out')
	lines = datafile.readlines()
	datafile.close()

	ax = []
	ay0 = []
	ay1 = []
	ay2 = []
	ay3 = []
	ay4 = []
	r0 = []
	r1 = []
	r2 = []
	header = lines[0].split(',')
	print header
	if isAll:
		heuristics = [header[6], header[7], header[8], header[9]]
	else:
		heuristics = [header[6], header[7], header[8], header[9], header[10]]
	for i in xrange(1, len(lines)):
		line = lines[i].strip().split(',')
		ax.append(float(line[4])/int(line[1]))
		#ax.append(float(line[4]))
		ay0.append(float(line[6]))
		ay1.append(float(line[7]))
		ay2.append(float(line[8]))
		ay3.append(float(line[9]))
		#if float(line[9]) > 0:
		#	r0.append(float(line[6])/float(line[9]))
		#	r1.append(float(line[7])/float(line[9]))
		#	r2.append(float(line[8])/float(line[9]))
		if not isAll:
			ay4.append(float(line[10]))
	print heuristics
	#print "%s %s %.2f"%(heuristics[0], heuristics[3], sum(r0)/len(r0))
	#print "%s %s %.2f"%(heuristics[1], heuristics[3], sum(r1)/len(r1))
	#print "%s %s %.2f"%(heuristics[2], heuristics[3], sum(r2)/len(r2))
	plt.xlabel('tamanho')
	plt.ylabel('tempo')
	plt.plot(ax, ay0, 'y*', label=heuristics[0])
	plt.plot(ax, ay1, 'bs', label=heuristics[1])
	plt.plot(ax, ay2, 'g^', label=heuristics[2])
	plt.plot(ax, ay3, 'ro', label=heuristics[3])
	if not isAll:
		plt.plot(ax, ay4, 'cx', label=heuristics[4])
	plt.legend(shadow=True, fancybox=True)
	if isAll:
		plt.savefig('allQuery.png')
	else:
		plt.savefig('begQuery.png')
