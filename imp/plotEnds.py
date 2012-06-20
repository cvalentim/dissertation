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
	datafile = open('begQuery.out')
	lines = datafile.readlines()
	datafile.close()

	ans = []
	ends = []
	x = []
	ratio = []
	for i in xrange(1, len(lines)):
		line = lines[i].split(',')
		ans.append(float(line[4]))
		ends.append(float(line[5]))
		x.append(i)
	for i in xrange(len(ans)):
		if ans[i] >= 10:
			ratio.append(ends[i]/ans[i])
	plt.ylabel('razao')
	plt.xlabel('consulta')
	plt.plot(ratio, 'bx')
	#plt.legend(shadow=True, fancybox=True)
	#plt.show()
	plt.savefig('begEndAns.png')
