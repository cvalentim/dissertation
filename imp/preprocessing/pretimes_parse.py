import sys
from collections import defaultdict


def is_pretime(line):
    try:
        float(line)
        return True
    except ValueError:
        return False


def main():
    m = defaultdict(list)
    lines = sys.stdin.readlines()
    for i in xrange(1, len(lines)):
        if is_pretime(lines[i - 1]):
            serie_id = int(lines[i].split(', ')[0])
            time = float(lines[i - 1])
            m[serie_id].append(time)
    print "SerieID H1, H2, H3"
    for serie_id in m:
        print str(serie_id),
        print m[serie_id]
        #for x in m[serie_id]:
        #    av += x
        #assert (len(m[serie_id] == 2)
        #print ", "
        #print ""


if __name__ == '__main__':
    main()
