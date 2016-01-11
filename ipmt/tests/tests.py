import os
import subprocess
import tempfile
import time
import random
import re

import matplotlib.pyplot as plt
from numpy import pi, sin, linspace
from matplotlib.mlab import stineman_interp
import matplotlib.collections as mcol
from matplotlib.lines import Line2D

random.seed(42)

def plot(data, figname) :
    fig, ax = plt.subplots()

    colors = ['blue', 'green', 'black', 'yellow', 'red', 'orange']
    styles = ['dashed', 'solid', 'dashed', 'dashed', 'dashed', 'solid']
    algs = []
    i = 0
    maxY = -1
    for a in data :
        algs.append(a)

        x = data[a][0]
        y = data[a][1]
        yp = None

        ax.plot(x, y, linestyle='ro', marker='o', c=colors[i], ls=styles[i], label=a)

        maxY = max(maxY, max(y))

        i += 1

    plt.ylabel('running time (ms)')
    plt.xlabel('pattern length')

    legend = plt.legend(loc='upper right', shadow=True)

    plt.savefig(figname)
    # plt.show()
    plt.clf()

def run_and_get_output(cmd):
    f = tempfile.TemporaryFile()
    start_time = time.time()
    proc = subprocess.Popen(cmd, stdout=f)
    proc.wait()
    rn_time = (time.time() - start_time)*1000 # Running time in ms
    r = (rn_time, '')
    return r

def generate_patterns(filename):
    patterns = []
    sizes = [1, 2, 3, 4, 5, 6, 8, 10, 12, 14, 15, 16, 20, 23, 26, 30, 32, 37, 40, 42, 48, 50, 52, 60, 70, 80, 83, 83, 90, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700]
    with open(filename, 'r') as myfile:
        text=myfile.read()
        length = len(text)
        for s in sizes:
            i = random.randint(0, length-s)
            p = text[i:i+s]
            patterns.append(p)
        myfile.close()
    return patterns

def generate_indexes(f, array, compress='lzw'):
    if not array:
        cmd = ['./ipmt', 'index']
    else:
        cmd = ['./ipmt', 'index', '-a']

    if compress == 'lz77':
        cmd.append('-compress=lz77')

    cmd.append(f)

    r = run_and_get_output(cmd)
    time = r[0]
    return time

def search(filename, patterns, grep):
    times = []
    for p in patterns:
        print len(p)
        if not grep:
            cmd = ['./ipmt', 'search', '-c', p, filename]
        else:
            cmd = ['grep', '-c', p, filename]
        t = run_and_get_output(cmd)
        times.append(t[0])
    return times

def run():
    filenameTree = 'english5-tree.txt'
    filenameArray = 'english5-array.txt'

    print 'Generating random patterns...'
    patterns = generate_patterns(filenameTree)

    sizes = [1, 2, 3, 4, 5, 6, 8, 10, 12, 14, 15, 16, 20, 23, 26, 30, 32, 37, 40, 42, 48, 50, 52, 60, 70, 80, 83, 83, 90, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700]

    pf = open('patterns.txt', 'w')
    for p in patterns:
        pf.write(p.replace('\n', ' ')+'\n')
    pf.close()
    print 'Done!'

    print 'LZW: Generating tree index...'
    timeIndexTree = generate_indexes(filenameTree, False)
    print 'Done! in %f ms' % timeIndexTree

    print 'LZW: Generating array index...'
    timeIndexArray = generate_indexes(filenameArray, True)
    print 'Done! in %f ms' % timeIndexArray

    print 'LZW: Searching using tree...'
    timeSearchTree = search(filenameTree[:len(filenameTree)-3]+'idx', patterns, False)
    print 'Done!'

    print 'LZW: Searching using array...'
    timeSearchArray = search(filenameArray[:len(filenameArray)-3]+'idx', patterns, False)
    print 'Done!'

    print 'LZW: Searching using grep...'
    timeSearchGrep = search(filenameTree, patterns, True)
    print 'Done!'

    dataTo70 = {
        'grep' : (
            sizes[:25],
            timeSearchGrep[:25]
        ),
        'tree' : (
            sizes[:25],
            timeSearchTree[:25]
        ),
        'array' : (
            sizes[:25],
            timeSearchArray[:25]
        )
    }
    plot(dataTo70, 'normal-lzw')

    dataSmall = {
        'grep' : (
            sizes[:12],
            timeSearchGrep[:12]
        ),
        'tree' : (
            sizes[:12],
            timeSearchTree[:12]
        ),
        'array' : (
            sizes[:12],
            timeSearchArray[:12]
        )
    }
    plot(dataSmall, 'pequenos-lzw')

    dataBig = {
        'grep' : (
            sizes[26:],
            timeSearchGrep[26:]
        ),
        'tree' : (
            sizes[26:],
            timeSearchTree[26:]
        ),
        'array' : (
            sizes[26:],
            timeSearchArray[26:]
        )
    }
    plot(dataBig, 'grandes-lzw')

    print 'LZ77: Generating tree index...'
    timeIndexTree = generate_indexes(filenameTree, False, compress="lz77")
    print 'Done! in %f ms' % timeIndexTree

    print 'LZ77: Generating array index...'
    timeIndexArray = generate_indexes(filenameArray, True, compress="lz77")
    print 'Done! in %f ms' % timeIndexArray

    print 'LZ77: Searching using tree...'
    timeSearchTree = search(filenameTree[:len(filenameTree)-3]+'idx', patterns, False)
    print 'Done!'

    print 'LZ77: Searching using array...'
    timeSearchArray = search(filenameArray[:len(filenameArray)-3]+'idx', patterns, False)
    print 'Done!'

    print 'LZ77: Searching using grep...'
    timeSearchGrep = search(filenameTree, patterns, True)
    print 'Done!'

    dataTo70 = {
        'grep' : (
            sizes[:25],
            timeSearchGrep[:25]
        ),
        'tree' : (
            sizes[:25],
            timeSearchTree[:25]
        ),
        'array' : (
            sizes[:25],
            timeSearchArray[:25]
        )
    }
    plot(dataTo70, 'normal-lz77')

    dataSmall = {
        'grep' : (
            sizes[:12],
            timeSearchGrep[:12]
        ),
        'tree' : (
            sizes[:12],
            timeSearchTree[:12]
        ),
        'array' : (
            sizes[:12],
            timeSearchArray[:12]
        )
    }
    plot(dataSmall, 'pequenos-lz77')

    dataBig = {
        'grep' : (
            sizes[26:],
            timeSearchGrep[26:]
        ),
        'tree' : (
            sizes[26:],
            timeSearchTree[26:]
        ),
        'array' : (
            sizes[26:],
            timeSearchArray[26:]
        )
    }
    plot(dataBig, 'grandes-lz77')

if __name__ == '__main__':
    # run_and_get_output(['./ipmt', 'index', '-a', 'desolation-row.txt'])
    # print run_and_get_output(['./ipmt', 'search', 'the', 'desolation-row.idx'])
    # print os.path.getsize('english-tree.txt')
    run()
