from elements.bench.base import AbstractBench

import matplotlib.pyplot as plt
import matplotlib as mpl
from matplotlib.ticker import FuncFormatter
import numpy as np
import os.path

class MemBandwidth(AbstractBench):
    def __init__(self, obj, bench_obj):
        self.obj = obj
        self.bench_obj = bench_obj

    def to_html(self):
        self.gen_images()
        wd = os.getcwd()

        header = "<h2 id='MEMBandwidth'>Memory Bandwidth</h2>"

        imgs = ""
        # imgs += f"<img src='{wd}/out/mem_bandwidth_single.svg'/>"
        imgs += f"<img src='{wd}/out/mem_bandwidth_multi.svg'/>"
        txt = "<p><span class='code-block'>memcpy()</span> of increasing sizes, performed on a single core then all of them. Multi-core results are stacked. Each core has its own buffer.</p>"
        
        return header + imgs + txt

    def gen_images(self):
        self.single_core_img()
        self.multi_core_img()

    def single_core_img(self):
        data = self.bench_obj["results"]

        runs = data["runs"]
        
        sizes = np.array(data["sizes"])
        reps = np.array(data["reps"])
        times = np.array(data["times"]["single"])

        avg = np.sum(times, axis=0) / runs
    
        x = sizes / 1024
        y = sizes * reps / avg

        _error = []
        for a in times:
            _error.append(sizes * reps / a)
        error = np.std(_error, axis=0)

        plt.figure(figsize=(10, 6))
        plt.errorbar(x, y, error, marker=".", ecolor="grey")

        plt.xscale("log", base=2)
        # matfloplib
        plt.gca().yaxis.set_major_formatter(FuncFormatter(lambda a, b: a))

        plt.xlabel("Buffer Size (KiB)")
        plt.ylabel("Bandwidth (GiB/s)")
        plt.title("Memory Bandwidth (single-core)")
        plt.grid(True, which="both", ls="--")

        caches = [
            int(self.obj["meta"]["mem_info"]["l1d"] / 1024),
            int(self.obj["meta"]["mem_info"]["l2"] / 1024),
            int(self.obj["meta"]["mem_info"]["l3"] / 1024)
        ]

        if 0 not in caches:
            plt.axvline(x=caches[0], color='green', linestyle='--', label=f'L1 size: {caches[0]} KiB')
            plt.axvline(x=caches[1], color='green', linestyle='--', label=f'L2 size: {caches[1]} KiB')
            plt.axvline(x=caches[2], color='green', linestyle='--', label=f'L3 size: {caches[2]} KiB')

        plt.legend()
        plt.savefig("out/mem_bandwidth_single.svg")
        plt.clf()

    def multi_core_img(self):
        
        data = self.bench_obj["results"]

        runs = data["runs"]
        
        sizes = np.array(data["sizes"])
        reps = np.array(data["reps"])
        times = np.array(data["times"]["multi"])
        single = np.array(data["times"]["single"])

        avg = np.sum(times, axis=0) / runs
        avg_single = np.sum(single, axis=0) / runs
    
        x = sizes / 1024
        y = sizes * reps / avg
        y_single = sizes * reps / avg_single

        _error = []
        for a in single:
            _error.append(sizes * reps / a)
        error = np.std(_error, axis=0)

        plt.figure(figsize=(10, 6))
        colors = [c for c in mpl.colormaps["Blues"](np.linspace(0.2, .8, num=4))]
        plt.stackplot(x, y, colors=colors, labels=["Multi core (stacked)"])
        plt.errorbar(x, y_single, error, marker=".", ecolor="grey", color="orange", label="Single core")

        plt.xscale("log", base=2)
        plt.gca().yaxis.set_major_formatter(FuncFormatter(lambda a, b: a))

        plt.xlabel("Buffer Size (KiB)")
        plt.ylabel("Bandwidth (GiB/s)")
        plt.title("Memory Bandwidth")
        plt.grid(True, which="both", ls="--")

        caches = [
            int(self.obj["meta"]["mem_info"]["l1d"] / 1024),
            int(self.obj["meta"]["mem_info"]["l2"] / 1024),
            int(self.obj["meta"]["mem_info"]["l3"] / 1024)
        ]

        if 0 not in caches:
            plt.axvline(x=caches[0], color='green', linestyle='--', label=f'L1 size: {caches[0]} KiB')
            plt.axvline(x=caches[1], color='green', linestyle='--', label=f'L2 size: {caches[1]} KiB')
            plt.axvline(x=caches[2], color='green', linestyle='--', label=f'L3 size: {caches[2]} KiB')

        plt.legend()
        plt.savefig("out/mem_bandwidth_multi.svg")
        plt.clf()

    def get_index(self):
        return "<li><a href='#MEMBandwidth'>Memory Bandwidth</a></li>"
