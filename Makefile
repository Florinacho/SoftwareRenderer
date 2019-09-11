CC=g++
FLAGS=-O3
#FLAGS=-g3
LIBS=-lm

.PHONY: clean

build: main.out

rebuild: clean build

main.out: main.o FrameBuffer.o Image.o Graphics.o Timer.o
	$(CC) $(FLAGS) FrameBuffer.o Image.o Graphics.o Timer.o main.o -o main.out $(LIBS)

main.o: main.cpp
	$(CC) $(FLAGS) -c main.cpp -o main.o

FrameBuffer.o: FrameBuffer.cpp
	$(CC) $(FLAGS) -c FrameBuffer.cpp -o FrameBuffer.o

Image.o: Image.cpp
	$(CC) $(FLAGS) -c Image.cpp -o Image.o
	
Graphics.o: Graphics.cpp
	$(CC) $(FLAGS) -c Graphics.cpp -o Graphics.o

Timer.o: Timer.cpp
	${CC} ${FLAGS} -c Timer.cpp -o Timer.o

clean:
	rm -f *.o FrameBuffer.o Image.o Graphics.o Timer.o main.out
