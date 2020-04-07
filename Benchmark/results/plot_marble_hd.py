# All credit for this file goes to Alexander Viand
import matplotlib as mpl
import numpy as np
import math
from matplotlib.backends.backend_pdf import PdfPages
from matplotlib.ticker import ScalarFormatter, FormatStrFormatter


def plot_figs(library, function):
    # figure settings
    fig_width_pt = 300                    # Get this from LaTeX using \showthe\columnwidth
    inches_per_pt = 1.0/72.27*2                 # Convert pt to inches
    golden_mean = ((math.sqrt(5)-1.0)/2.0)*.8   # Aesthetic ratio
    fig_width = fig_width_pt*inches_per_pt      # width in inches
    fig_height =(fig_width*golden_mean)           # height in inches
    fig_height = fig_height/1.5
    fig_size = [fig_width,fig_height]

    params = {'backend': 'ps',
        'axes.labelsize': 20,
        'font.size': 16,
        'axes.titlesize': 24,
        'legend.fontsize': 16,
        'xtick.labelsize': 16,
        'ytick.labelsize': 16,
        'figure.figsize': fig_size,
        'font.family': 'times new roman'}

    mpl.rcParams.update(params)


    import matplotlib.pyplot as plt
    data = np.genfromtxt('granite_' + function + '_' + library + '.csv', delimiter=',', names=True)
    #plt.plot([1,2,3,4])

    begin = 0
    end = 60

    plt.plot(data['n'][begin:end], data['batched'][begin:end], color='0.2', label='Hamming Distance',linestyle='-',marker='o',markersize=5, linewidth=2)
    #plt.plot(data['max_levels'], data['time_add'], color='c', label='Addition',linestyle='-',marker='o',markersize=4)
    #plt.title("Comparison of Adder Performance")
    plt.xlabel('Elements in vector')
    plt.ylabel('Time [s]')
    plt.gca().xaxis.set_major_formatter(FormatStrFormatter('%.0f'))
    plt.gca().yaxis.set_major_formatter(FormatStrFormatter('%.0f'))
    plt.legend(loc=2)
    plt.grid(True, linestyle=':', color='0.8', zorder=0)
    plt.xlim(0,60)
    plt.ylim(-0.1,2)#36

    plt.rc('text', usetex=True)
    plt.rc('font', family='serif')

    F = plt.gcf()
    F.set_size_inches(fig_size)
    #plt.show()
    #plt.savefig('granite_' + function + '_' + library + '_batched.png')
    pdf_pages = PdfPages('granite_' + function + '_' + library + '_batched.pdf')
    pdf_pages.savefig(F,bbox_inches='tight')
    plt.clf()
    pdf_pages.close()

    begin = 0
    end = 30

    plt.plot(data['n'][begin:end], data['slow'][begin:end], color='0.2', label='Hamming Distance',linestyle='-',marker='o',markersize=5, linewidth=2)
    #plt.plot(data['max_levels'], data['time_add'], color='c', label='Addition',linestyle='-',marker='o',markersize=4)
    #plt.title("Comparison of Adder Performance")
    plt.xlabel('Elements in vector')
    plt.ylabel('Time [s]') #min
    plt.gca().xaxis.set_major_formatter(FormatStrFormatter('%.0f'))
    plt.gca().yaxis.set_major_formatter(FormatStrFormatter('%.0f'))
    plt.legend(loc=2)
    plt.grid(True, linestyle=':', color='0.8', zorder=0)
    #plt.yticks(range(0,35001,5000), range(0,36,5))
    plt.xlim(0,30)
    plt.ylim(-0.1,6) #4.2

    plt.rc('text', usetex=True)
    plt.rc('font', family='serif')

    F = plt.gcf()
    F.set_size_inches(fig_size)
    #plt.show()
    #plt.savefig('granite_' + function + '_' + library + '.png')
    pdf_pages = PdfPages('granite_' + function + '_' + library + '.pdf')
    pdf_pages.savefig(F,bbox_inches='tight')
    plt.clf()
    pdf_pages.close()


plot_figs('SEALBFV','hd')


