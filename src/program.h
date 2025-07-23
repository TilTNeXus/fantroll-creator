#pragma once
#include <SDL3/SDL.h>

#include <stdbool.h>
#include <string.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define AMT_ACC_BACK 29
#define AMT_ACC_CLOTH 30
#define AMT_ACC_FACE 52
#define AMT_ACC_HATS 33
#define AMT_BANGS 98
#define AMT_BG 20
#define AMT_BODY 4
#define AMT_BOTTOM 101
#define AMT_EYE 72
#define AMT_EYEBROWS 23
#define AMT_FACEPAINT 32
#define AMT_HAIR 165
#define AMT_HORN 183
#define AMT_MARKINGS 17
#define AMT_MOUTH 62
#define AMT_SHOE 73
#define AMT_SIGIL 301
#define AMT_UPPER 244

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GLES3/gl3.h>
#endif

#include "stb_image.h"
#include "stb_image_write.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "mat4f.h"
#include "camera.h"
#include "text.h"

extern SDL_Window *window;
extern uint64_t ticks;
extern bool isGLES;
extern float deltaTime;

typedef struct textureinfo {
    unsigned int tex;
    unsigned int width;
    unsigned int height;
} textureinfo;

typedef struct layertex {
    textureinfo lines;
    textureinfo tint;
    char *layername;
    int asset;
    float r;
    float g;
    float b;
} layertex;
typedef struct mouseState {
    float x;
    float y;
    bool held;
    bool clicked;
    bool released;
} mouseState;
extern mouseState cursor;
#include "mouse.h"

typedef struct keyState {
    bool left;
    bool right;
    bool up;
    bool down;
    bool enter;
} keyState;
typedef struct keyStatus {
    keyState pressed;
    keyState held;
    keyState released;
    mouseState mouse;
} keyStatus;
extern keyStatus keys;
extern mat4f model;
extern mat4f view;
extern mat4f projection;
extern vec3f cameraPos;
extern vec3f cameraFront;
extern vec3f cameraUp;
extern vec3f cameraPos;

typedef struct modelinfo {
    float *verts;
    unsigned int size;
    unsigned int vao;
    unsigned int vbo;
    textureinfo texture;
} modelinfo;
extern modelinfo models[16];
extern modelinfo textmodel;
extern char activeScreen[16];
typedef struct framebuffer {
    unsigned int fbo, tex, rbo, vao, vbo, shader;
} framebuffer;
extern framebuffer screenFB;
extern framebuffer creatorFB;
extern unsigned int shaderProgram[5];
extern unsigned int textShader;

extern layertex acc_back;
extern layertex acc_cloth;
extern layertex acc_face;
extern layertex acc_hats;
extern layertex bangs;
extern layertex bg;
extern layertex body;
extern layertex bottom;
extern layertex eyeL;
extern layertex eyeR;
extern layertex eyebrows;
extern layertex facepaint;
extern layertex hair;
extern layertex hornL;
extern layertex hornR;
extern layertex markings;
extern layertex mouth;
extern layertex shoeL;
extern layertex shoeR;
extern layertex sigil;
extern layertex upper;

typedef struct character {
    unsigned int textureID, outlineID;
    unsigned int sizeX, sizeY;
    int bearingX, bearingY;
    unsigned int advance;    
} character;
extern character characters[128];
extern int activeCategory;
extern int activeColorMode;
extern layertex *editingLayer;
extern float cursorX;
extern float cursorY;
extern bool buttonsClicked[48];
extern int clicked;
extern float cursorSpeed;