#include "program.h"
#include "titlescreen.h"

void loadCreator(void) {
    strcpy(activeScreen, "creator");
}

void inputCreator(void) {
    if (keys.pressed.enter) {
        keys.mouse.clicked = 1;
        exportTrollImage();
    }
    if (keys.mouse.clicked) {
        clicked = detectUIButtons(cursor);
    }
    if (keys.mouse.released) {
        updateClickedButtons(clicked);
    }
    float cursorSpeed = 200.0 * deltaTime;
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

void drawCreator(void) {

    mat4f_init(model);
    mat4f_trans_translate(model, 0.0, 0.0, -20.0);
    glUniform1f(glGetUniformLocation(shaderProgram[0], "opacity"), 0.0);
    loadUniformsAndDrawTextured(models[3], shaderProgram[0], models[3].texture.tex);

    drawCreatorTroll();

    mat4f_init(model);
    mat4f_trans_translate(model, 0.0, 0.0, -10.0);
    loadUniformsAndDrawTexturedOffset(models[5], shaderProgram[0], models[5].texture.tex, models[5].size*activeCategory/3, models[5].size/3);

    drawNumbers(models[6], shaderProgram[2]);
    drawArrows(models[7], shaderProgram[2]);
    drawCategories(models[8], shaderProgram[2]);
    drawCursor(models[7], shaderProgram[2]);
}

void controlCreator(void) {
    inputCreator();
    drawCreator();
}