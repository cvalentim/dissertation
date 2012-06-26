"""
When plotting time series, eg financial time series, one often wants
to leave out days on which there is no data, eh weekends.  The example
below shows how to use an 'index formatter' to achieve the desired plot
"""
import sys
import pylab
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
		datafile = open('allPretime.out')
	else:
		datafile = open('begPretime.out')
	lines = datafile.readlines()
	datafile.close()
	
	fig = pylab.figure()
	ax = range(13)[1:13]
	ay = []
	header = lines[0].split(',')
	colors = ['r', 'b', 'y', 'g']
	for i in xrange(1, len(lines)):
		line = lines[i].split(',')
		p = fig.add_subplot(len(lines) - 1, 1, i)
		print "ax = ", ax
		ay = [float(x) for x in line[1:]]
		print "ay = ", ay
		p.bar(ax, ay, facecolor=colors[i-1], align='center', label=line[0])
		pylab.xticks(range(13))
		plt.legend(prop={'size':12})
		#if isAll:
			#pylab.ylim([0, 500])
		#else:
			#pylab.ylim([0, 6000])
	if isAll:
		plt.savefig('allPretime.png')
	else:
		plt.savefig('begPretime.png')
