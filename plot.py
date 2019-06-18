import matplotlib.pyplot as plt
from os import listdir, system, remove, mkdir
from os.path import isfile, join, exists

system("g++ -std=c++11 bat.cpp -o bat")
system(".\\bat.exe")

graphingFiles = [join("./graphs/", f) for f in listdir("./graphs/") if isfile("./graphs/"+f) and "GraphAux" in f]

if(not exists("./graphs/")):
    mkdir("./graphs/")

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

    subplot2 = plt.subplot(122)
    subplot2.title.set_text("Zoomed in on Bats")
    subplot2.scatter(xdata, ydata)

    plt.savefig(filename.replace("Aux", "").replace(".txt", ".png"))
    plt.close()
    remove(filename)
    