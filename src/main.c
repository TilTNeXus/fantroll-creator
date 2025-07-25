#define SDL_MAIN_USE_CALLBACKS
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define GL_SILENCE_DEPRECATION

#include <stdio.h>
#include <stdlib.h>

//#include <emscripten.h>
#include <SDL3/SDL_main.h>

#include "program.h"
#include "loading.h"
#include "titlescreen.h"
#include "creator.h"
#include "colors.h"

SDL_Window *window = NULL;
SDL_GLContext context = NULL;
bool running;
uint64_t ticks;
float deltaTime = 0.0;
bool isGLES = false;
char activeScreen[16] = "creator";
character characters[128];
float cursorX;
float cursorY;
bool buttonsClicked[64] = {0};
float cursorSpeed;

mouseState cursor;
keyStatus keys;

int activeCategory = 0;
int activeColorMode = 1;
layertex *editingLayer = NULL;
int sliderX[3];
vec3f originalColor;

framebuffer screenFB;
framebuffer creatorFB;
unsigned int textShader;
unsigned int shaderProgram[5];
modelinfo models[16];
modelinfo textmodel;

layertex acc_back;
layertex acc_cloth;
layertex acc_face;
layertex acc_hats;
layertex bangs;
layertex bg;
layertex body;
layertex bottom;
layertex eyeL;
layertex eyeR;
layertex eyebrows;
layertex facepaint;
layertex hair;
layertex hornL;
layertex hornR;
layertex markings;
layertex mouth;
layertex shoeL;
layertex shoeR;
layertex sigil;
layertex upper;

int hemo[12] =  {   0xa10000,
                    0xa75403,
                    0xa1a100,
                    0x6b8400,
                    0x406600,
                    0x078446,
                    0x008282,
                    0x004182,
                    0x0021cb,
                    0x5e00a0,
                    0x6a006a,
                    0xb11262
                };

mat4f model;
mat4f view;
mat4f projection;
vec3f cameraPos = {0.0, 0.0, 3.0};
vec3f cameraFront = { 0.0, 0.0, -1.0};
vec3f cameraUp = {0.0, 1.0, 0.0};

SDL_AppResult SDL_AppEvent (void *appstate, SDL_Event *event) {
    switch (event->type) {
        case SDL_EVENT_QUIT:
            running = 0;
            return SDL_APP_SUCCESS;
            break;
        case SDL_EVENT_KEY_DOWN:
            switch (event->key.key) {
                case SDLK_LEFT:
                    keys.pressed.left = 1;
                    keys.held.left = 1;
                    break;
                case SDLK_DOWN:
                    keys.pressed.down = 1;
                    keys.held.down = 1;
                    break;
                case SDLK_RIGHT:
                    keys.pressed.right = 1;
                    keys.held.right = 1;

                    break;
                case SDLK_UP:
                    keys.pressed.up = 1;
                    keys.held.up = 1;
                    break;
                case SDLK_RETURN:
                    keys.pressed.enter = 1;
                    keys.held.enter = 1;
                    break;
                default:
                    break;
            }
            break;
        case SDL_EVENT_KEY_UP:
            switch (event->key.key) {
                case SDLK_LEFT:
                    keys.released.left = 1;
                    keys.held.left = 0;
                    break;
                case SDLK_DOWN:
                    keys.released.down = 1;
                    keys.held.down = 0;
                    break;
                case SDLK_RIGHT:
                    keys.released.right = 1;
                    keys.held.right = 0;
                    break;
                case SDLK_UP:
                    keys.released.up = 1;
                    keys.held.up = 0;
                    break;
                case SDLK_RETURN:
                    keys.released.enter = 1;
                    break;             
                default:
                    break;
            }
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            SDL_GetMouseState(&keys.mouse.x, &keys.mouse.y);
            keys.mouse.clicked = 1;
            keys.mouse.held = 1;
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            keys.mouse.held = 0;
            keys.mouse.released = 1;
            for (int i = 0; i < 64; i++) {
                buttonsClicked[i] = 0;
            }
            break;   
        case SDL_EVENT_FINGER_DOWN:
            keys.mouse.clicked = 1;
            SDL_WarpMouseInWindow(window, event->tfinger.x * SCREEN_WIDTH, event->tfinger.y * SCREEN_HEIGHT);
        case SDL_EVENT_FINGER_MOTION:
            keys.mouse.held = 1;
            SDL_WarpMouseInWindow(window, event->tfinger.x * SCREEN_WIDTH, event->tfinger.y * SCREEN_HEIGHT);
            break;
        case SDL_EVENT_FINGER_UP:
            keys.mouse.held = 0;
            keys.mouse.released = 1;
            cursor.x = keys.mouse.x;
            cursor.y = SCREEN_HEIGHT - keys.mouse.y;
            break;
        default:
            break;
    }
    return SDL_APP_CONTINUE;
}

void initDrawing(void) {

    textShader = shaderLoad("assets/shaders/text.vs", "assets/shaders/text.fs");
    glUseProgram(textShader);
    glUniform1i(glGetUniformLocation(textShader, "text"), 0);
    loadFont("assets/fonts/courierstuck.ttf");
    glGenVertexArrays(1, &textmodel.vao);
    glGenBuffers(1, &textmodel.vbo);
    glBindVertexArray(textmodel.vao);
    glBindBuffer(GL_ARRAY_BUFFER, textmodel.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*6*4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    textureLoad(&models[0].texture, "assets/titles/title screen.png", GL_RGB);
    createRectFromTexture(&models[0], models[0].texture);
    loadIntoVAO(&models[0]);
    shaderProgram[0] = shaderLoad("assets/shaders/textured.vs", "assets/shaders/textured.fs");
    glUseProgram(shaderProgram[0]);
    glUniform1i(glGetUniformLocation(shaderProgram[0], "tex"), 0);

    textureLoad(&models[1].texture, "assets/ui/window.png", GL_RGBA);
    modelLoad(&models[1], "assets/models/uiwindow.obj");
    modifyUIWindow(&models[1], 160, 96);
    loadIntoVAO(&models[1]);

    textureLoad(&models[2].texture, "assets/ui/window.png", GL_RGBA);
    modelLoad(&models[2], "assets/models/selectionoverlay.obj");
    modifySelectionOverlay(&models[2], 135, 25);
    loadIntoVAO(&models[2]);

    char categoryTilesetPath[32];
    snprintf(categoryTilesetPath, 32, "assets/tilesets/Doll%d_A2.png", activeCategory+1);
    textureLoad(&models[3].texture, categoryTilesetPath, GL_RGBA);
    modelLoad(&models[3], "assets/models/makerui.obj");
    loadIntoVAO(&models[3]);

    textureinfo creatorRect;
    textureLoad(&creatorRect, "assets/layers/transparent.png", GL_RGBA);
    createRectFromTexture(&models[4], creatorRect);
    loadIntoVAO(&models[4]);
    shaderProgram[1] = shaderLoad("assets/shaders/textured.vs", "assets/shaders/creator.fs");
    glUseProgram(shaderProgram[1]);
    glUniform1i(glGetUniformLocation(shaderProgram[1], "linesTex"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram[1], "tintTex"), 1);

    textureLoad(&models[5].texture, "assets/ui/buttons2.png", GL_RGBA);
    modelLoad(&models[5], "assets/models/buttons2.obj");
    loadIntoVAO(&models[5]);

    textureLoad(&models[6].texture, "assets/ui/numbers.png", GL_RGBA);
    modelLoad(&models[6], "assets/models/32x32.obj");
    loadIntoVAO(&models[6]);
    shaderProgram[2] = shaderLoad("assets/shaders/textured.vs", "assets/shaders/offsetuvs.fs");
    
    textureLoad(&models[7].texture, "assets/ui/buttons1.png", GL_RGBA);
    modelLoad(&models[7], "assets/models/32x32.obj");
    loadIntoVAO(&models[7]);

    textureLoad(&models[8].texture, "assets/ui/categs.png", GL_RGBA);
    modelLoad(&models[8], "assets/models/64x64.obj");
    loadIntoVAO(&models[8]);

    if (activeColorMode == 0) {
    textureLoad(&models[9].texture, "assets/tilesets/RGB_A2.png", GL_RGBA);
    } else if (activeColorMode == 1) {
        textureLoad(&models[9].texture, "assets/tilesets/HSL_A2.png", GL_RGBA);
    } else {
        textureLoad(&models[9].texture, "assets/tilesets/HEX_A2.png", GL_RGBA);
    }
    modelLoad(&models[9], "assets/models/colorui.obj");
    loadIntoVAO(&models[9]);

    modelLoad(&models[10], "assets/models/coloruibuttons.obj");
    loadIntoVAO(&models[10]);

    textureLoad(&models[11].texture, "assets/ui/presets.png", GL_RGBA);
    createRectFromTexture(&models[11], models[11].texture);
    loadIntoVAO(&models[11]);

    acc_back.layername = "acc_back";
    bg.layername = "bg";
    hair.layername = "hair";
    body.layername = "body";
    markings.layername = "markings";
    facepaint.layername = "facepaint";
    mouth.layername = "mouth";
    eyebrows.layername = "eyebrows";
    eyeL.layername = "eye";
    eyeR.layername = "eye";
    shoeL.layername = "shoe";
    shoeR.layername = "shoe";
    bottom.layername = "bottom";
    acc_cloth.layername = "acc_cloth";
    upper.layername = "upper";
    sigil.layername = "sigil";
    acc_face.layername = "acc_face";
    hornL.layername = "horn";
    hornR.layername = "horn";
    bangs.layername = "bangs";
    acc_hats.layername = "acc_hats";

    loadCreatorTexture(&acc_back, SDL_rand(AMT_ACC_BACK+1), SDL_randf(), SDL_randf(), SDL_randf());
    loadCreatorTexture(&bg, SDL_rand(AMT_BG+1), SDL_randf(), SDL_randf(), SDL_randf());
    loadCreatorTexture(&hair, SDL_rand(AMT_HAIR+1), SDL_randf(), SDL_randf(), SDL_randf());
    loadCreatorTexture(&body, SDL_rand(AMT_BODY)+1, SDL_randf(), SDL_randf(), SDL_randf());
    loadCreatorTexture(&markings, SDL_rand(AMT_MARKINGS+1), SDL_randf(), SDL_randf(), SDL_randf());
    loadCreatorTexture(&facepaint, SDL_rand(AMT_FACEPAINT+1), SDL_randf(), SDL_randf(), SDL_randf());
    loadCreatorTexture(&mouth, SDL_rand(AMT_MOUTH+1), SDL_randf(), SDL_randf(), SDL_randf());
    loadCreatorTexture(&eyebrows, SDL_rand(AMT_EYEBROWS+1), SDL_randf(), SDL_randf(), SDL_randf());
    loadCreatorTexture(&eyeL, SDL_rand(AMT_EYE+1), SDL_randf(), SDL_randf(), SDL_randf());
    loadCreatorTexture(&eyeR, SDL_rand(AMT_EYE+1), SDL_randf(), SDL_randf(), SDL_randf());
    loadCreatorTexture(&shoeL, SDL_rand(AMT_SHOE+1), SDL_randf(), SDL_randf(), SDL_randf());
    loadCreatorTexture(&shoeR, SDL_rand(AMT_SHOE+1), SDL_randf(), SDL_randf(), SDL_randf());
    loadCreatorTexture(&bottom, SDL_rand(AMT_BOTTOM+1), SDL_randf(), SDL_randf(), SDL_randf());
    loadCreatorTexture(&acc_cloth, SDL_rand(AMT_ACC_CLOTH+1), SDL_randf(), SDL_randf(), SDL_randf());
    loadCreatorTexture(&upper, SDL_rand(AMT_UPPER+1), SDL_randf(), SDL_randf(), SDL_randf());
    loadCreatorTexture(&sigil, SDL_rand(AMT_SIGIL+1), SDL_randf(), SDL_randf(), SDL_randf());
    loadCreatorTexture(&acc_face, SDL_rand(AMT_ACC_FACE+1), SDL_randf(), SDL_randf(), SDL_randf());
    loadCreatorTexture(&hornL, SDL_rand(AMT_HORN+1), SDL_randf(), SDL_randf(), SDL_randf());
    loadCreatorTexture(&hornR, SDL_rand(AMT_HORN+1), SDL_randf(), SDL_randf(), SDL_randf());
    loadCreatorTexture(&bangs, SDL_rand(AMT_BANGS+1), SDL_randf(), SDL_randf(), SDL_randf());
    loadCreatorTexture(&acc_hats, SDL_rand(AMT_ACC_HATS+1), SDL_randf(), SDL_randf(), SDL_randf());

    mat4f_init(projection);
    mat4f_trans_orthographic(projection, 0.0, SCREEN_WIDTH, 0.0, SCREEN_HEIGHT, 0.1, 100.0);
    //mat4f_trans_perspective(projection, M_PI / 2.0, (float)SCREEN_WIDTH/SCREEN_HEIGHT, 0.1, 100.0);

    glEnable(GL_BLEND);
    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);

    createFramebuffer(&screenFB, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    glUniform1i(glGetUniformLocation(screenFB.shader, "tex"), 0);

    createFramebuffer(&creatorFB, 288, 416, -288);
}

void renderScene(void) {
    cameraMovement();
    vec3f viewCenter;
    vec3f_add(viewCenter, cameraPos, cameraFront);
    mat4f_lookat(view, cameraPos, viewCenter, cameraUp);
    glCullFace(GL_BACK);

    if (strcmp(activeScreen, "title") == 0) controlTitleScreen();
    else if (strcmp(activeScreen, "creator") == 0) controlCreator();
    else if (strcmp(activeScreen, "colors") == 0) controlColors();

}

void draw(void) {
    glBindFramebuffer(GL_FRAMEBUFFER, screenFB.fbo);
    glClearColor(0.8f, 0.8f, 0.8f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    renderScene();

    glDisable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    
    glUseProgram(screenFB.shader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, creatorFB.tex);
    glBindVertexArray(creatorFB.vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glBindTexture(GL_TEXTURE_2D, screenFB.tex);
    glBindVertexArray(screenFB.vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    SDL_GL_SwapWindow(window);
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
    running = 1;
    SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "0");
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    window = SDL_CreateWindow("Fantroll Creator", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    context = SDL_GL_CreateContext(window);
    
    #ifdef EMSCRIPTEN
    isGLES = true;
    #else 
    glewInit();
    #endif
    
    SDL_HideCursor();
    
    initDrawing();
    SDL_srand(SDL_GetTicks());

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    Uint64 currentTicks = SDL_GetTicks();
    deltaTime = (float)(currentTicks - ticks) / 1000.0;
    ticks = currentTicks;
    float mouseX = keys.mouse.x;
    float mouseY = keys.mouse.y;
    SDL_GetMouseState(&keys.mouse.x, &keys.mouse.y);
    if (mouseX != keys.mouse.x || mouseY != keys.mouse.y) {
        cursor.x = keys.mouse.x;
        cursor.y = SCREEN_HEIGHT - keys.mouse.y;
    }
    cursorSpeed = 240.0 * deltaTime;
    draw();
    keys.pressed.left = 0;
    keys.pressed.down = 0;
    keys.pressed.right = 0;
    keys.pressed.up = 0;
    keys.pressed.enter = 0;

    keys.released.left = 0;
    keys.released.down = 0;
    keys.released.right = 0;
    keys.released.up = 0;
    keys.released.enter = 0;

    keys.mouse.clicked = 0;
    keys.mouse.released = 0;
    while(SDL_GetTicks()-ticks < 16) SDL_Delay(1);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}