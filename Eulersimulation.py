#Simulation file for Euler equation
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

#Now I will just make a temporary read file function. This file can read a text file and reshape it into a numpy array used to make the simulation
print("Reading data file...")
with open('data.txt','r') as f:
    data= np.loadtxt(f) #taking the text file's members into an array.

shape = np.shape(data)
unique_times=np.unique(data[:,0])
number_of_timesteps = len(unique_times)
number_of_spacesteps = shape[0] // number_of_timesteps

#Now I will reshape the data. I ONLY UNDERSTAND THIS VAGUELY, MAKE SURE TO READ AND THINK ON THIS
reshaped_data = data.reshape(number_of_timesteps, int(number_of_spacesteps),int(shape[1]))

t_values= reshaped_data[:,0,0]
x_values= reshaped_data[0,:,1]
rho_values= reshaped_data[:,:,2]
u_values= reshaped_data[:,:,3]
p_values= reshaped_data[:,:,4]

#Create the still pictures.
fig, (ax1, ax2, ax3) = plt.subplots(3,1,figsize=(11,13)) #Plt gives back a whole figure object and a list of 3 plots in this case

line1, = ax1.plot(x_values, rho_values[0,:], 'b-', linewidth=2)
ax1.set_xlim(-2,2)  #Later make this more dynamic
ax1.set_ylim(-2,3)
ax1.set_xlabel('x')
ax1.set_ylabel('rho')
ax1.grid(True)

line2, = ax2.plot(x_values, u_values[0,:], 'r-', linewidth=2)
ax2.set_xlim(-2,2)
ax2.set_ylim(-2,3)
ax2.set_xlabel('x')
ax2.set_ylabel('u')
ax2.grid(True)

line3, = ax3.plot(x_values, p_values[0,:], 'g-', linewidth=2)
ax3.set_xlim(-2,2)
ax3.set_ylim(-2,3)
ax3.set_xlabel('x')
ax3.set_ylabel('p')
ax3.grid(True)

plt.tight_layout() #Always use this for more than one plot, so that they are not jumbled up

def update(frame):
    line1.set_ydata(rho_values[frame,:])
    line2.set_ydata(u_values[frame,:])
    line3.set_ydata(p_values[frame,:])
    ax1.set_title(f'Euler Equations, Time = {t_values[frame]:.2f}s')
    return line1, line2, line3,

print('Creating Animation')
animation = FuncAnimation(fig,update,frames=len(t_values),interval= 100,blit=True) #interval is time between two frames in milliseconds
plt.show()
animation.save('Euler.gif', writer='ffmpeg',fps=20,dpi=100)