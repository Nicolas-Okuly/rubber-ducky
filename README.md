# Rubber Ducky

# Dependencies
You can run the following commands to install all the dependecies:
```sh
sudo apt update
sudo apt install libxrandr-dev libxcursor-dev libxi-dev libudev-dev libfreetype-dev libflac-dev libvorbis-dev libgl1-mesa-dev libegl1-mesa-dev libfreetype-dev libsfml-dev
```

# Compiling
You can run the following command to compile your code:
```sh
g++ main.cpp -o duck -lsfml-graphics -lsfml-window -lsfml-system
```

Only tested on linux thus far
