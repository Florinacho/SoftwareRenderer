OUTPUT=main.exe

CC=mingw32-g++.exe

C_FLAGS=-O3# -msse3 -ffast-math -march=native
#C_FLAGS=-g3
L_FLAGS=-O3# -msse3 -ffast-math -march=native

#L_FLAGS+=-Wl,-subsystem,windows

LIBS=-lgdi32

CORE_SOURCE=../Core
C_FLAGS+=-I$(CORE_SOURCE)

SOURCE_FILES=\
			$(CORE_SOURCE)/Image.cpp \
			$(CORE_SOURCE)/Timer.cpp \
			$(CORE_SOURCE)/Window.cpp \
			GUI.cpp \
			main.cpp
			
OBJECT_FILES = $(SOURCE_FILES:.cpp=.o)

build: $(OUTPUT)

clean:
	rm -rf $(OBJECT_FILES) $(OUTPUT)

rebuild: clean build

%.o: %.cpp
	$(CC) $(C_FLAGS) -c $< -o $@

$(CORE_SOURCE)/%.o: $(CORE_SOURCE)/%.cpp
	$(CC) $(C_FLAGS) -c $< -o $@

$(OUTPUT): $(OBJECT_FILES)
	$(CC) $(L_FLAGS) $(OBJECT_FILES) $(LIBS) -o $(OUTPUT)
	chmod +xr $(OUTPUT)