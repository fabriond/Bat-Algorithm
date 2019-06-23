import matplotlib.pyplot as plt
from os import listdir, system, remove, mkdir
from os.path import isfile, join, exists
import numpy as np

if(not exists("./plots/")):
    mkdir("./plots/")

system("cd cpp_code && g++ -std=c++11 main.cpp bat/bat.cpp random/random_engine.cpp -o main && .\\main.exe")

graphingFiles = [join("./plots/", f) for f in listdir("./plots/") if isfile("./plots/"+f) and "PlotAux" in f]

for filename in graphingFiles:
    xdata = []
    ydata = []
    with open(filename) as f:
        for line in f: # read rest of lines
            x, y = [float(n) for n in line.split(",")]
            xdata.append(x)
            ydata.append(y)
    
    plt.figure(figsize=[12.8, 4.8])

    subplot1 = plt.subplot(121)
    subplot1.set_xlim(-100, 100)
    subplot1.set_ylim(-100, 100)
    subplot1.title.set_text("Search Space Sized")
    subplot1.scatter(xdata, ydata)
    subplot1.grid(which='both', linestyle='-.')

    subplot2 = plt.subplot(122)
    subplot2.title.set_text("Zoomed in on Bats")
    subplot2.scatter(xdata, ydata)
    subplot2.grid(which='both', linestyle='-.')

    plt.savefig(filename.replace("Aux", "").replace(".txt", ".png"))
    plt.close()
    remove(filename)
