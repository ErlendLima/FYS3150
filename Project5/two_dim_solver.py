import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import seaborn as sns
sns.set()

fig = plt.figure()

N = 31
h = 1.0/(N-1)
t = 1
t_steps = 1000000
dt = 0.000001
alpha = dt / h**2
u = np.zeros(shape=(N,N))
n_figs = 200
fig_every = t_steps / n_figs

u[:,0] = u[0,:] = 1
u[:,-1] = u[-1,:] = 0
u_new = np.zeros(u.shape, type(u[0,0]))

ims = []

for k in range(t_steps):
    u_new[1:-1,1:-1] = u[1:-1,1:-1] + alpha*(u[2:,1:-1]
                                             - 4*u[1:-1,1:-1]
                                             + u[:-2,1:-1]
                                             + u[1:-1,2:]
                                             + u[1:-1,:-2])

    if k % fig_every == 0:
        im = plt.imshow(u_new[1:-1,1:-1], cmap='hot', animated=True, interpolation='bicubic')
        text = plt.text(0.1, 0.9, str(k), bbox={'facecolor': 'white',
                                              'alpha': 0.5,
                                              'pad': 5})
        ims.append([im,text])
    temp = u_new
    u_new = u
    u = temp

ani = animation.ArtistAnimation(fig, ims, interval=50, blit=True,
                                repeat_delay=1000)
plt.show()
