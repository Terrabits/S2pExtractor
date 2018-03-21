#!/usr/bin/env python3

from   skrf import Network
from matplotlib import pyplot as plt
from pathlib import Path
import sys

filename = Path(__file__).absolute().parent / sys.argv[1]
filename = str(filename)
print(filename)

n1 = Network(filename)
n1.s11.plot_s_db(label='S11 dB')
n1.s21.plot_s_db(label='S21 dB')
n1.s21.plot_s_deg(label='S21 deg')
n1.s22.plot_s_db(label="S22 dB")

plt.show()
