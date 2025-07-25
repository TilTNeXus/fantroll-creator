OBJS = src/main.c src/mat4f.c src/vec3f.c src/camera.c src/loading.c src/titlescreen.c src/mouse.c src/text.c src/creator.c src/colors.c src/drawing.c
NAME = fantroll\ creator

COMPILER_FLAGS = -Wall

LIBRARY = $$(pkg-config --cflags --libs sdl3 freetype2)

INCLUDE = -I./include

CC_EM = emcc

PRELOAD = --preload-file ./assets

all : $(OBJS)
	$(CC) -O3 $(OBJS) $(COMPILER_FLAGS) $(LIBRARY) $(INCLUDE) -lSDL3 -lm -lGL -lGLEW -o $(NAME)
wasm : $(OBJS)
	$(CC_EM) -O3 $(OBJS) $(COMPILER_FLAGS) -sUSE_SDL=3 -I./include $(PRELOAD) -s USE_WEBGL2=1 -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 -s USE_FREETYPE=1 -o $(NAME).html
mac : $(OBJS)
	$(CC) -O3 -Wno-deprecated-declarations $(OBJS) $(COMPILER_FLAGS) $(LIBRARY) $(INCLUDE) -framework OpenGL -lGLEW -o $(NAME)
