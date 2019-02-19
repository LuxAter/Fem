import matplotlib.pyplot as plt
import numpy as np
import sys

if len(sys.argv) > 1:
    if sys.argv[1] == 'save':
        for i in range(0,9):
            data = np.loadtxt("{}.txt".format(i))
            plt.imshow(data)
            plt.imsave("{}.png".format(i), data)
    elif sys.argv[1] == 'join':
        data = []
        for i in range(5, 9):
            data.append(np.loadtxt("{}.txt".format(i)))
        total = sum(data)
        print(total)
        plt.imshow(total)
        plt.imsave("join.png", total)
    else:
        data = np.loadtxt(sys.argv[1] + '.txt')
        plt.imshow(data)
        plt.show()
else:
    for i in range(0,9):
        data = np.loadtxt("{}.txt".format(i))
        plt.imshow(data)
        plt.show()

