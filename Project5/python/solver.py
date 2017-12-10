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

def solve(N=101, n_steps=3001, subplots=False, plot_amplitude=True):
    u0 = np.zeros(shape=(N,N))
    u = np.zeros(shape=(N,N))
    u0 = set_initial_conditions(u0, N)

    dx = 1/(N-1)
    dt = dx**2 / 4
    print("dt = {}".format(dt))
    alpha = dt/dx**2
    print("alpha = {}".format(alpha))

    if subplots:
        figsteps = [0,1000,2000,3000]
        fignum = 0
        fig = plt.figure()

    middle = N//2
    middle_amp = np.zeros(n_steps+1); middle_amp[0] = u0[middle,middle]

    for k in range(n_steps):
        u0, u = step(u0, u, alpha)

        if subplots:
            if k in figsteps:
                fignum += 1
                ax = fig.add_subplot(220 + fignum)
                im = ax.imshow(u.copy(), cmap=plt.get_cmap('gist_heat'), vmin=0, vmax=1,
                               interpolation='bicubic')
                ax.set_axis_off()
                ax.set_title('t = {:.2f}'.format(k*dt))

        if plot_amplitude:
            middle_amp[k+1] = u[middle,middle]

    if subplots:
        fig.subplots_adjust(right=0.85)
        cbar_ax = fig.add_axes([0.9, 0.15, 0.03, 0.7])
        cbar_ax.set_xlabel('T', labelpad=20)
        fig.colorbar(im, cax=cbar_ax)
        plt.savefig('../latex/figures/twodim.eps')
        plt.show()

    if plot_amplitude:
        fig2 = plt.figure()
        ax = fig2.add_subplot(111)
        ax.plot(np.linspace(0, n_steps*dt, n_steps+1, endpoint=True), middle_amp)
        ax.set_xlabel("t")
        ax.set_ylabel(r"Center node solution amplitude $u$")
        plt.savefig("../latex/figures/twodimamplitude.eps")
        plt.show()

if __name__ == "__main__":
    solve(n_steps=10000)
