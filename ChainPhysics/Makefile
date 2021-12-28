OUTPUT=main.exe

CC=mingw32-g++.exe

C_FLAGS=-O3 -msse3 -ffast-math -march=native
#C_FLAGS=-g3
L_FILES=-O3 -msse3 -ffast-math -march=native

LIBS=-lm -pthread -lgdi32

COMMON_SOURCE=../Core
C_FLAGS+=-I$(COMMON_SOURCE)

SOURCE_FILES=\
			$(COMMON_SOURCE)/Image.cpp \
			$(COMMON_SOURCE)/FrameBuffer.cpp \
			$(COMMON_SOURCE)/Timer.cpp \
			$(COMMON_SOURCE)/Input.cpp \
			$(COMMON_SOURCE)/Window.cpp \
			main.cpp
			
OBJECT_FILES = $(SOURCE_FILES:.cpp=.o)

build: $(OUTPUT)

clean:
	rm -rf $(OBJECT_FILES) $(OUTPUT)

rebuild: clean build

%.o: %.cpp
	$(CC) $(C_FLAGS) -c $< -o $@

$(COMMON_SOURCE)/%.o: $(COMMON_SOURCE)/%.cpp
	$(CC) $(C_FLAGS) -c $< -o $@

$(OUTPUT): $(OBJECT_FILES)
	$(CC) $(L_FILES) $(OBJECT_FILES) $(LIBS) -o $(OUTPUT)
	chmod +xr $(OUTPUT)