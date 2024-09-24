from matplotlib import cm
#from matplotlib.ticker import LinearLocator
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
 
data = pd.read_csv('Data_2.txt', sep = ' ', header=None)
data = pd.DataFrame(data)

fig, ax = plt.subplots(subplot_kw={"projection": "3d"})

x = data[0]
y = data[1]
t = data[2]

#plt.xlabel('X')
#plt.ylabel('Y')
#plt.tlabel('T')

#plt.plot(x, y)
#plt.title('Line Graph using CSV')
X, Y = np.meshgrid(x, y)
t = np.array(t)
t_2d = [t]
size = len(t)
for i in range(1, len(t)):
    t = np.roll(t,-1)
    t_2d.append(t)

    for j in range(i):
        t_2d[i][size-1-j] = np.nan 

t_2d = np.array(t_2d)
#print(t_2d)

# Plot the surface.
surf = ax.plot_surface(X, Y, np.array(t_2d), cmap=cm.coolwarm)

#surf = ax.plot_surface(X, Y, R, cmap=cm.coolwarm,
#                       linewidth=0, antialiased=False)
#print(X)
#print(Y)
#plt.grid()

plt.show()