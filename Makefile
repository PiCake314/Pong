main: main.cpp
	g++ -c main.cpp -ID:\Documents\libraries\SFML-2.5.1\include && g++ main.o -o app -L D:\Documents\libraries\SFML-2.5.1\lib -lsfml-graphics -lsfml-window -lsfml-system