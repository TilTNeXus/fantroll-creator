#include "program.h"
#include "drawing.h"
#include "titlescreen.h"

int editingSlider = -1;

void loadColors(void) {
    strcpy(activeScreen, "colors");
    originalColor[0] = editingLayer->r;
    originalColor[1] = editingLayer->g;
    originalColor[2] = editingLayer->b;
}

void inputColors(void) {
    if (keys.pressed.enter) {
        keys.mouse.clicked = 1;
    }
    if (keys.mouse.clicked) {
        clicked = detectUIButtonsColor(cursor);
        if (mouseInBox(cursor, 0, SCREEN_WIDTH, 96, 128)) editingSlider = 0;
        else if (mouseInBox(cursor, 0, SCREEN_WIDTH, 64, 96)) editingSlider = 1;
        else if (mouseInBox(cursor, 0, SCREEN_WIDTH, 32, 64)) editingSlider = 2;
    }
    if (keys.mouse.held) {
        updateSlidersColor(cursor, editingLayer);
    }
    if (keys.mouse.released) {
        updateClickedButtonsColor(editingLayer, clicked);
        editingSlider = -1;
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

void drawColors(void) {

    mat4f_init(model);
    mat4f_trans_translate(model, 0.0, 0.0, -20.0);
    glUniform1f(glGetUniformLocation(shaderProgram[0], "opacity"), 0.0);
    loadUniformsAndDrawTextured(models[9], shaderProgram[0], models[9].texture.tex);

    glUseProgram(shaderProgram[1]);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram[1], "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram[1], "projection"), 1, GL_FALSE, projection);

    mat4f_init(model);
    mat4f_trans_scale(model, 0.5, 0.5, 1.0);
    mat4f_trans_translate(model, 56.0, 216.0, -11.0);
    drawCreatorLayer(editingLayer);

    drawNumbersColors(models[6], *editingLayer, shaderProgram[2]);
    drawArrowsColors(models[7], shaderProgram[2]);
    mat4f_init(model);
    mat4f_trans_translate(model, 0.0, 0.0, -10.0);
    loadUniformsAndDrawTexturedOffset(models[10], shaderProgram[0], models[5].texture.tex, models[10].size*activeColorMode/3, models[10].size/3);
    loadUniformsAndDrawTextured(models[11], shaderProgram[0], models[11].texture.tex);
    /*
    drawCreatorTroll();

    drawNumbers(models[6], shaderProgram[2]);
    drawArrows(models[7], shaderProgram[2]);
    drawCategories(models[8], shaderProgram[2]);
    */
    glDisable(GL_DEPTH_TEST);
    drawCursor(models[7], shaderProgram[2]);
}

void controlColors(void) {
    inputColors();
    drawColors();
}