#include "program.h"
#include "drawing.h"
#include "titlescreen.h"
#include "creator.h"

uint8_t titleSelection = 1;
int clicked = -1;
int fadeStart = 750;
int fadeLength = 500;

void loadTitleScreen(void) {

}

void inputTitleScreen(void) {
    if (keys.pressed.down) {
        titleSelection = (titleSelection + 1)%3;
    } 
    if (keys.pressed.up) {
        titleSelection = moduint(titleSelection-1, 3);
    }
    if (keys.pressed.enter) {
        keys.mouse.clicked = 1;
        if (titleSelection == 2) {
            SDL_Event quit;
            quit.type = SDL_EVENT_QUIT;
            SDL_PushEvent(&quit);
        }
    }
    if (keys.mouse.clicked) {
        for (int i = 0; i < 3; i++) {
            int selectionBoxY = 190 + 24*i;
            if (mouseInBox(keys.mouse, 47, 182, selectionBoxY, selectionBoxY+25)) {
                titleSelection = i;
            }
        }
    }
    if (keys.mouse.released) {
        if (titleSelection == 0) {
            loadCreator();
        } else if (titleSelection == 2) {
            SDL_Event quit;
            quit.type = SDL_EVENT_QUIT;
            SDL_PushEvent(&quit);
        }
    }
    if (keys.held.down) {
        float updated = cursor.y - cursorSpeed;
        if (0 <= updated) cursor.y = updated;
        SDL_WarpMouseInWindow(window, cursor.x, SCREEN_HEIGHT - cursor.y);
    }
    if (keys.held.up) {
        float updated = cursor.y + cursorSpeed;
        if (updated <= SCREEN_HEIGHT) cursor.y = updated;
        SDL_WarpMouseInWindow(window, cursor.x, SCREEN_HEIGHT - cursor.y);
    }
    if (keys.held.left) {
        float updated = cursor.x - cursorSpeed;
        if (0 <= updated) cursor.x = updated;
        SDL_WarpMouseInWindow(window, cursor.x, SCREEN_HEIGHT - cursor.y);
    }
    if (keys.held.right) {
        float updated = cursor.x + cursorSpeed;
        if (updated <= SCREEN_WIDTH) cursor.x = updated;
        SDL_WarpMouseInWindow(window, cursor.x, SCREEN_HEIGHT - cursor.y);
    }
}

void drawTitleScreen(void) {
    // background
    
    mat4f_init(model);
    mat4f_trans_translate(model, 0.0, 0.0, -50.0);
    glUseProgram(shaderProgram[0]);
    glUniform1f(glGetUniformLocation(shaderProgram[0], "opacity"), 0.0);
    loadUniformsAndDrawTextured(models[0], shaderProgram[0], models[0].texture.tex);

    // uiwindow
    int scaleAnimStart = 1125;
    int scaleAnimLength = 125;
    mat4f_init(model);
    if (scaleAnimStart <= ticks) {
        int windowHeightHalf = 96/2;
        if (ticks <= scaleAnimStart+scaleAnimLength) {
            mat4f_trans_translate(model, 0.0, -windowHeightHalf, 0.0);
            mat4f_trans_scale(model, 1.0, (float)(ticks-scaleAnimStart)/scaleAnimLength, 1.0);
            mat4f_trans_translate(model, 0.0, windowHeightHalf, 0.0);
        }
        mat4f_trans_translate(model, 35.0, 207.0, -8.0);
        loadUniformsAndDrawTextured(models[1], shaderProgram[0], models[1].texture.tex);
    }

    // selectionoverlay and text
    if (scaleAnimStart+scaleAnimLength < ticks) {
        mat4f_init(model);
        mat4f_trans_translate(model, 47.0, 267.0 - 24*titleSelection, -7.0);
        glUniform1f(glGetUniformLocation(shaderProgram[0], "opacity"), 1.0 - (0.25*cosf(3*M_PI*ticks/1000)+0.75));
        loadUniformsAndDrawTextured(models[2], shaderProgram[0], models[2].texture.tex);

        float textcolor[3] = {1.0, 0.709, 0.157};
        drawText(textShader, "New troll", 52.0, 273.0, 1.0, textcolor);
        drawText(textShader, "Load troll", 52.0, 249.0, 1.0, textcolor);
        drawText(textShader, "Exit", 52.0, 225.0, 1.0, textcolor);
    }
    glDisable(GL_DEPTH_TEST);
    drawCursor(models[7], shaderProgram[2]);

    // framebuffer
    glUseProgram(screenFB.shader);
    if (ticks <= fadeStart) {
        glUniform1f(glGetUniformLocation(screenFB.shader, "darkness"), 1.0);
    }
    else if (fadeStart < ticks && ticks <= fadeStart+fadeLength) {
        glUniform1f(glGetUniformLocation(screenFB.shader, "darkness"), 1.0 - (float)(ticks-fadeStart)/fadeLength);
    } else {
        glUniform1f(glGetUniformLocation(screenFB.shader, "darkness"), 0.0);
    }
}

void controlTitleScreen(void) {
    inputTitleScreen();
    //printf("%f %f\n", keys.mouse.x, keys.mouse.y);

    drawTitleScreen();
}