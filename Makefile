
all: app

complete: clean app clean

dir:
	mkdir objs

app: objs/Model.o objs/Camera.o objs/Game.o objs/glad.o objs/main.o objs/shader.o objs/Texture.o objs/Material.o
	g++ objs/Model.o objs/Camera.o objs/Game.o objs/Material.o objs/glad.o objs/Texture.o objs/main.o objs/shader.o -lglfw3 -lgdi32 -llibsoil2 -lzlibstatic -lassimp -o app.exe -g -fdiagnostics-color=always -Wall
	app

objs/Model.o: src/Model.cpp
	g++ -c $< -o $@

objs/Camera.o: src/Camera.cpp
	g++ -c $< -o $@

objs/Game.o: src/Game.cpp
	g++ -c $< -o $@

objs/main.o: src/main.cpp
	g++ -c $< -o $@

objs/glad.o: src/glad.c
	g++ -c $< -o $@

objs/shader.o: src/shader.cpp
	g++ -c $< -o $@

objs/Texture.o: src/Texture.cpp
	g++ -c $< -o $@

objs/Material.o: src/Material.cpp
	g++ -c $< -o $@

clean:
	del /f objs\*.o app.exe