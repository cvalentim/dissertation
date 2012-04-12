import sys
from collections import defaultdict


def is_query_line(line):
    if line.startswith('SerieID'):
        return False
    if line.strip() == "":
        return False
    if line.count(',') <= 8:
        return False
    return True


def parse_line(line):
    return line.strip(' ').split(', ')


def get_key(t_line):
    # SerieID, Heuristic,  SerieSize, t,    d,    ans,  av_time, min,  med,  max, ends
    # (serie_id, serie_size, t, d, ans)
    #print >>sys.stderr, ', '.join(t_line)
    return (int(t_line[0]), int(t_line[2]), float(t_line[3]), float(t_line[4]), int(t_line[5]))


def get_ends(t_line):
    return t_line[10].strip()


def get_med_time(t_line):
    return float(t_line[8])


def get_heuristic(t_line):
    return t_line[1]


def print_table(m, ends_table):
    print "idserie, size, t, d, ans, ends, ",
    header_print = False
    for key in sorted(m.keys()):
        exec_times = [x[1] for x in m[key]]
        heuristics = [x[0] for x in m[key]]
        if not header_print:
            header_print = True
            print ', '.join(heuristics)
        print "%d, %d, %.2f, %.2f, %d, %s" % (key[0], key[1], key[2], key[3], key[4], ends_table[key]),
        for x in exec_times:
            print ", %.2f " % x,
        print ""


def main():
    # expect input through stdin
    bigtable = defaultdict(list)
    ends_table = defaultdict(str)
    lines = sys.stdin.readlines()
    for line in lines:
        if not is_query_line(line):
            continue
        parsed_line = parse_line(line)
        key = get_key(parsed_line)
        h = get_heuristic(parsed_line)
        took = get_med_time(parsed_line)
        bigtable[key].append((h, took))
        if get_ends(parsed_line).isdigit():
            ends_table[key] = get_ends(parsed_line)
    nh = 0
    for key in bigtable:
        bigtable[key] = sorted(bigtable[key])
        if nh == 0:
            nh = len(bigtable[key])
            print >>sys.stderr, nh
            continue
        assert nh == len(bigtable[key])
    print_table(bigtable, ends_table)

if __name__ == '__main__':
    main()
