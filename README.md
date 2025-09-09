# Rubber Ducky
Powered by SFML.

# Dependencies
You can run the following commands to install all the dependecies:
```sh
sudo apt update
sudo apt install libxrandr-dev libxcursor-dev libxi-dev libudev-dev libfreetype-dev libflac-dev libvorbis-dev libgl1-mesa-dev libegl1-mesa-dev libfreetype-dev libsfml-dev
```

## Windows
It's easier for me to just include them all with the repo.

# Compiling
You can run the following command to compile your code:
```sh
g++ main.cpp -o duck -lsfml-graphics -lsfml-window -lsfml-system
```

## Windows
You can compile using the following command:
```sh
g++ main.cpp -o duck.exe -I ./include -L ./lib -lsfml-graphics -lsfml-window -lsfml-system -mwindows
```
