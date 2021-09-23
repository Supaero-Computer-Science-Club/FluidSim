# FluidSim
2D Fluid Simulation

# Dependencies

## SFML lib
For Debian/Ubuntu distro:
```bash
sudo apt-get install libsfml-dev
```

For Arch Linux based distro:
```bash
sudo pacman -S sfml
```

## cmake
For Ubuntu, use the snap package manager
```bash
sudo snap install cmake
```

For Arch Linux:
```bash
sudo pacman -S cmake
```

# Compilation
If you compile the project for the first time, run the following command in the source directory of the project
```bash
mkdir build
```

Then, run the following commands to compile the project 
```bash
cd build
cmake ..
make
./app
```

# Controls
Use the following keys to control the simulation:
- Space: Switch on/off the step mode
- Enter: In step mode only, make one simulation step
- r: reset the simulation

