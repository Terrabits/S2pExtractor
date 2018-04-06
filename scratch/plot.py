#!/usr/bin/env python3

from   skrf import Network
from matplotlib import pyplot as plt
from pathlib import Path
import sys

files = sys.argv[1:]
if not files:
    print('Usage: plot.py <filename1>[,<filename2>,...]')
    sys.exit(1)

root = Path(__file__).absolute().parent
files = [str(root / file) for file in files]

plt.figure(1)
for file in files:
    n = Network(file)
    plt.subplot(221) # s11
    plt.plot(n.s11.f, n.s11.s_db.flatten())
    plt.subplot(222) # s22
    plt.plot(n.s22.f, n.s22.s_db.flatten())
    plt.subplot(223) # s21 (dB)
    plt.plot(n.s21.f, n.s21.s_db.flatten())
    plt.subplot(224) # s21 (deg)
    plt.plot(n.s21.f, n.s21.s_deg.flatten())

plt.show()
