CC=g++
FLAGS=-O3# -Wall
#FLAGS=-g3
LIBS=-lm

.PHONY: clean

build: main.out

rebuild: clean build

main.out: main.o Image.o FrameBuffer.o RenderTarget.o Renderer.o Shader.o FBRenderer.o Timer.o
	$(CC) $(FLAGS) Image.o FrameBuffer.o RenderTarget.o Renderer.o Shader.o FBRenderer.o Timer.o main.o -o main.out $(LIBS)

main.o: main.cpp
	$(CC) $(FLAGS) -c main.cpp -o main.o

Image.o: Image.cpp
	$(CC) $(FLAGS) -c Image.cpp -o Image.o

FrameBuffer.o: FrameBuffer.cpp
	$(CC) $(FLAGS) -c FrameBuffer.cpp -o FrameBuffer.o

RenderTarget.o : RenderTarget.cpp
	$(CC) $(FLAGS) -c RenderTarget.cpp -o RenderTarget.o
	
Renderer.o: Renderer.cpp
	$(CC) $(FLAGS) -c Renderer.cpp -o Renderer.o

Shader.o: Shader.cpp
	$(CC) $(FLAGS) -c Shader.cpp -o Shader.o

FBRenderer.o: FBRenderer.cpp
	$(CC) $(FLAGS) -c FBRenderer.cpp -o FBRenderer.o

Timer.o: Timer.cpp
	${CC} ${FLAGS} -c Timer.cpp -o Timer.o

clean:
	rm -f *.o Image.o FrameBuffer.o RenderTarget.o Renderer.o Shader.o FBRenderer.o Timer.o main.out
