#Simulation file for Burger's equation
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
#number_of_spacesteps = (len(data)/number_of_timesteps) #len gives me the length of the first dimension of any array, 
# that is, in this case the row number
#Now I will reshape the data. I ONLY UNDERSTAND THIS VAGUELY, MAKE SURE TO READ AND THINK ON THIS
reshaped_data = data.reshape(number_of_timesteps, int(number_of_spacesteps),int(shape[1]))




t_values= reshaped_data[:,0,0]
x_values= reshaped_data[0,:,1]
q_values= reshaped_data[:,:,2]


#Create the still pictures.
fig, ax1 = plt.subplots(1,1,figsize=(11,13)) #Plt gives back a whole figure object and a list of 1 plot in this case
line1, = ax1.plot(x_values, q_values[0,:], 'b-', linewidth=2)
ax1.set_xlim(-10,10)  #Later make this more dynamic
ax1.set_ylim(0.0,3)
ax1.set_xlabel('x')
ax1.set_ylabel('q')
ax1.grid(True)






plt.tight_layout() #Always use this for more than one plot, so that they are not jumbled up

def update(frame):
    line1.set_ydata(q_values[frame,:])
    ax1.set_title(f'Wave Propagation, Time = {t_values[frame]:.2f}s')
    return line1,


print('Creating Animation')
animation = FuncAnimation(fig,update,frames=len(t_values),interval= 100,blit=True) #interval is time between two frames in milliseconds

plt.show()
animation.save('Burgers.gif', writer='ffmpeg',fps=20,dpi=100)