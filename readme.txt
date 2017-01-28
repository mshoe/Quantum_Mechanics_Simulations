QM_Simulations by Michael Xu
2017-01-28

This program presents a visualization of the famous quantum physics case: a particle in a 2d infinite potential well.
This program uses the equation |Psi*||Psi| to determine the probability of finding the particle at a particular position.
This is visualized with a red mesh. The higher and redder a part of the mesh is, the higher the chance of finding the particle.

The full equation used is:
|Psi*||Psi| = P(x, y, t), where P is the probability function
Psi = C1 * Psi_1 + C2 * Psi_2 + ... + C6 * Psi_6, where C1, C2, ..., C6 are constants from [0, 1] defined by the user with the sliders.
Psi_n = psi_n * phi_n
psi_n(x, y) = 2/sqrt(Lx * Ly) * sin( nx * 3.14159 * x / Lx) * sin(ny * 3.14159 * y / Ly), in this build, nx = ny for simplicity
Lx and Ly are the widths and lengths of the box.
phi_n(t) = exp( (-i * En / hbar) * t), where hbar is Planck's constant h / (2 * pi)
En = (h^2) / (8 * m) * ((nx * nx) / (Lx * Lx) + (ny * ny) / (Ly * Ly)), where m is the mass of the particle

Basic instructions:
Change the constants C1, C2, ..., C6, m, Lx, and Ly with the sliders
SPACE = pause/unpase
ESCAPE = close the program. I recommend using escape instead of the 'X' button.
LEFT/RIGHT = rotate

For background, I recommend this wiki page: https://en.wikipedia.org/wiki/Schr%C3%B6dinger_equation
For better background, I recommend the textbook: Modern Physics, Randy Harris
Even better, an introductory course to quantum physics.

