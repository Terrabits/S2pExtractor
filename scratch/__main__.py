from   skrf import Network
from matplotlib import pyplot as plt


n1 = Network('port1.s2p')
n1.s21.plot_s_db(label='P1 dB')
n1.s21.plot_s_deg(label='P1 deg')

# n2 = Network('port2.s2p')
# n2.s21.plot_s_db(label='P2 dB')
# n2.s21.plot_s_deg(label='P2 deg')

plt.show()
