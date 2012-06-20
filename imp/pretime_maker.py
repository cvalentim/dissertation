import sys
from collections import defaultdict

def isQueryLine(line):
	return line.strip().startswith('#pretime')

def parse_line(line):
	line = line.strip()
	line = line.replace('>', ' ');
	line = line.replace(':', ' ');
	fields = line.split(' ')[1:]
	return (fields[1], fields[2], fields[3], fields[4])

def get_time(t_line):
	return float(t_line[3])

def get_heuristic(t_line):
	return t_line[2]

def get_serieID(t_line):
	return t_line[0]

def get_size(t_line):
	return t_line[1]

def print_table(bigtable, serieIDs, _heuristics):
	series = sorted(serieIDs, key=int)
	heuristics = sorted(_heuristics)
	#print >>sys.stderr, "bigtable = ", bigtable
	#print >>sys.stderr, "serieIDs = ", serieIDs
	#print >>sys.stderr, "heuristics = ", heuristics
	header = ['Heuristic'] + [x for x in series]
	print ','.join(header)
	for h in heuristics:
		row = [h]
		for s in series:
			key = (h, s)
			n = len(bigtable[key])
			total = sum(bigtable[key])
			row.append('%.2f'%(total/n))
			#print "%.2f"%(total/n),
		print ','.join(row)

def main():
	# expect input through stdin
	bigtable = defaultdict(list)
	idToSize = defaultdict(int)
	serieIDs = set()
	heuristics = set()
	lines = sys.stdin.readlines()
	for line in lines:
		if not isQueryLine(line):
			continue
		parsed_line = parse_line(line)
		#key = get_key(parsed_line)
		h = get_heuristic(parsed_line)
		took = get_time(parsed_line)
		size = get_size(parsed_line)
		serieID = get_serieID(parsed_line)
		idToSize[serieID] = size
		key = (h, serieID)
		bigtable[key].append(float(took))
		serieIDs.add(serieID)
		heuristics.add(h)
	print_table(bigtable, serieIDs, heuristics)

if __name__ == '__main__':
	main()
