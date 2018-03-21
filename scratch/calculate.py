
from matplotlib import pyplot as plt
import numpy as np
from numpy import angle, average, complex_, conj, loadtxt, log10, ones, pi, sqrt, unwrap, zeros
from pathlib import Path

root = Path('Fred 3/port1')

converter   = lambda x: complex(x.decode())
filename    = lambda x: str(root / x)
loadcomplex = lambda x: loadtxt(filename(x), dtype=complex_,converters={0: converter})
dB          = lambda x: 20*log10(abs(x))
deg         = lambda x: angle(x) * 180.0/pi
wrap        = lambda x: (x + pi) % (2 * pi) - pi

out_dir = loadcomplex('outer_directivity.csv')
out_frq = loadtxt(filename("outer_frequency.csv"))
out_src = loadcomplex('outer_sourcematch.csv')
out_ref = loadcomplex('outer_refltrack.csv')

in_dir = loadcomplex('inner_directivity.csv')
in_frq = loadtxt(filename("inner_frequency.csv"))
in_src = loadcomplex('inner_sourcematch.csv')
in_ref = loadcomplex('inner_refltrack.csv')

assert(len(out_frq) == len(in_frq))
for i in range(0, len(out_frq)):
    assert(out_frq[i] == in_frq[i])

freq = out_frq

d   = out_ref + (out_src * (in_dir - out_dir))
s11 =          (in_dir - out_dir) / d
s21 =      sqrt(out_ref * in_ref) / d
s22 = in_src - (out_src * in_ref) / d

def fix_phase_jumps(s21):
    for i in range(1,len(s21)):
        delta = abs(angle(s21[i]*conj(s21[i-1])))
        if delta > pi/2.0:
            s21[i] = -1.0 * s21[i]
    return s21

def middle_range(s21):
    points = len(s21)
    if points < 2:
        return range(0,0)
    if points < 7:
        return range(1, points)
    start  = int(0.2 * points) + 1
    stop   = int(0.8 * points)
    return range(start, stop)
# y = mx + b
# b = y - mx
# m = (y2 - y1) / (x2 - x1)
# b = y1 - m*x1
def fix_dc(s21):
    middle = middle_range(s21)
    if not middle:
        return s21
    phases = unwrap(angle(s21))
    b_values = []
    for i in middle:
        m = (phases[i] - phases[i-1]) / (freq[i] - freq[i-1])
        b_values.append(phases[i] - m * freq[i])
    b_avg = average(b_values) % (2.0*pi)
    if b_avg > pi:
        b_avg = b_avg - 2.0 * pi
    print('b_avg: {0}'.format(b_avg))
    if abs(b_avg) > pi/2.0:
        print('Adjusting by 180 deg')
        s21 = -1.0*s21
    return s21

if __name__ == 'main':
    plt.figure(1)
    plt.subplot(411)
    plt.plot(freq, dB(s11))

    plt.subplot(412)
    plt.plot(freq, dB(s21))

    plt.subplot(413)
    plt.plot(freq, angle(fix_dc(fix_phase_jumps(s21))))

    plt.subplot(414)
    plt.plot(freq, dB(s22))

    plt.show()
