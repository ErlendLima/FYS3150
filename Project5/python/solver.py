import numpy as np
import matplotlib.pyplot as plt

def set_initial_conditions(u0, N):
    for i in range(1,N-1):
        for j in range(1,N-1):
            u0[i,j] = np.sin(np.pi*i/N)*np.sin(np.pi*j/N)
    return np.copy(u0)

def step(u0, u, alpha):
    u[1:-1, 1:-1] = u0[1:-1, 1:-1] + alpha*(u0[2:, 1:-1]
                                            - 4*u0[1:-1, 1:-1]
                                            + u0[:-2, 1:-1]
                                            + u0[1:-1, 2:]
                                            + u0[1:-1, :-2])
    u0 = u.copy()
    return u0, u

N = 101 # Lattice size
u0 = np.zeros(shape=(N,N))
u = np.zeros(shape=(N,N))
u0 = set_initial_conditions(u0, N)

dx = 1/(N-1)
dt = dx**2 / 4
alpha = dt/dx**2

n_steps = 3001
figsteps = [0,1000,2000,3000]
fignum = 0
fig = plt.figure()

for k in range(n_steps):
    u0, u = step(u0, u, alpha)
    if k in figsteps:
        fignum += 1
        ax = fig.add_subplot(220 + fignum)
        im = ax.imshow(u.copy(), cmap=plt.get_cmap('gist_heat'), vmin=0, vmax=1,
                       interpolation='bicubic')
        ax.set_axis_off()
        ax.set_title('t = {:.2f}'.format(k*dt))

fig.subplots_adjust(right=0.85)
cbar_ax = fig.add_axes([0.9, 0.15, 0.03, 0.7])
cbar_ax.set_xlabel('T', labelpad=20)
fig.colorbar(im, cax=cbar_ax)
plt.show()
