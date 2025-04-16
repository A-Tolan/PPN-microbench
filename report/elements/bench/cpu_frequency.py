from elements.bench.base import AbstractBench

import matplotlib.pyplot as plt
import numpy as np
import os.path
from matplotlib.ticker import MultipleLocator

class CpuFrequency(AbstractBench):
    def __init__(self, obj, bench_obj):
        self.obj = obj
        self.bench_obj = bench_obj

    def to_html(self):
        self.gen_images()
        wd = os.getcwd()

        header = "<h2 id='CPUFrequency'>CPU Frequency</h2>"

        imgs = f"<img src='{wd}/out/cpu_frequency_multiplot.png'/>"
        title = "<center><p>Frequencies of cores in GHz depending of their number</p></center>"
        
        return header+ imgs + title

    def gen_images(self):
        data = self.bench_obj["results"]

        div = 0
        for k in range(8, 1, -1) :
            if len(data) > k and len(data)%k == 0 :
                div = k
                break

        fig, truc = plt.subplots(div, (len(data)//div), figsize=(10,12))
        plt.subplots_adjust(hspace=0.35, wspace=0.1)
        # plt.figtext(0.5, 0.01, "Frequencies of cores in GHz depending of their number", horizontalalignment = 'center', size='x-large')

        # max_val = 0
        # min_val = 100

        # for cpu in data:
        #     for vals in data[cpu]:
        #         max_val = max(max_val, max(*vals))
        #         min_val = min(min_val, min(*vals))

        # max_val *= 1.1
        # min_val *= 0.9

        all_vals = []

        for j, cpu in enumerate(data):
            values = []
            # std = []
            if j == 0 :
                mot = ' Core'
            else :
                mot = ' Cores'
            for vals in data[cpu]:
                temp = []
                for val in vals:
                    temp.append(val)  
                # values.append(np.mean(temp))
                # std.append(np.std(temp))
                # all_vals.append(values[-1])

                np.array(temp)

                minimum = np.min(temp)
                q1 = np.percentile(temp, 25)
                median = np.median(temp)
                q3 = np.percentile(temp, 75)
                maximum = np.max(temp)

                values.append(temp)




            if div != 1 :
                cadre = truc[j//((len(data)//div)), j%((len(data)//div))] 
            else :
                cadre = truc[j//((len(data)//div))]

            cadre.boxplot(values)
        #     cadre.errorbar(range(1, j + 2), values, yerr=std, marker=".", label=str(j+1) + mot, capsize=5, capthick=1, ecolor='gray')
        #     cadre.set_ylim(min_val, max_val)
        #     # if warning :
        #     #     cadre.fill_between(range(j+1), top, bot, color='red', alpha=0.1)
        #     # else :
        #     #     cadre.fill_between(range(j+1), top, bot, color='blue', alpha=0.1)
            cadre.set_xlabel('Cores')
            cadre.set_ylabel('Frequency (GHz)')
        #     # cadre.ticklabel_format(useOffset=False)
        #     cadre.legend(loc="lower left", fontsize=10)
            cadre.set_ylim(bottom=0)
            cadre.grid(True, which='major', axis='both', linestyle='--', alpha=0.7)
        #     cadre.xaxis.set_major_locator(MultipleLocator(1))

        # if (np.std(all_vals) / np.mean(all_vals) * 100 > 5.0) :
        #     plt.figtext(0.5, 0.035, "/!\ Warning /!\ Standard deviation is too high : {:.2f}%".format(np.std(all_vals) / np.mean(all_vals) * 100), horizontalalignment = 'center', color="red")
        # else :
        #     plt.figtext(0.5, 0.035, "Standard deviation is : {:.2f}%".format(np.std(all_vals) / np.mean(all_vals) * 100), horizontalalignment = 'center')

        

        plt.tight_layout(pad=3.5)

        plt.savefig("out/cpu_frequency_multiplot.png")
        plt.clf()
        plt.close()

    def get_index(self):
        return "<li><a href='#CPUFrequency'>CPU Frequency</a></li>"
