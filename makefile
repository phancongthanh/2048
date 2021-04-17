All:Game\2048.exe

Game\2048.exe: main.o Code.o SDL_lib.o
	g++ -o Game\2048.exe main.o Code.o SDL_lib.o -LSDL\SDL2-2.0.14\x86_64-w64-mingw32\lib -lmingw32 -lSDL2main -lSDL2 -s -mwindows
main.o: main.cpp
	g++ -c main.cpp -ISDL\SDL2-2.0.14\x86_64-w64-mingw32\include\SDL2
Code.o: Code.cpp
	g++ -c Code.cpp -ISDL\SDL2-2.0.14\x86_64-w64-mingw32\include\SDL2
SDL_lib.o: SDL_lib.cpp
	g++ -c SDL_lib.cpp -ISDL\SDL2-2.0.14\x86_64-w64-mingw32\include\SDL2

clean:
	del main.o Code.o SDL_lib.o