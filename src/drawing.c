#include "drawing.h"
#include "loading.h"

void loadUniformsAndDraw(modelinfo mdl, unsigned int shader) {
    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, projection);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, model);
    glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, cameraPos);
    glBindVertexArray(mdl.vao);
    glDrawArrays(GL_TRIANGLES, 0, mdl.size);
}

void loadUniformsAndDrawTextured(modelinfo mdl, unsigned int shader, unsigned int texture) {
    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, projection);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, model);
    glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, cameraPos);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(mdl.vao);
    glDrawArrays(GL_TRIANGLES, 0, mdl.size);
}

void drawCreatorLayer(layertex *layer) {
    mat4f_trans_translate(model, 0.0, 0.0, 1.0);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram[1], "model"), 1, GL_FALSE, model);
    vec3f color = {layer->r, layer->g, layer->b};
    glUniform3fv(glGetUniformLocation(shaderProgram[1], "color"), 1, color);
    if (layer == &eyeR || layer == &shoeR || layer == &hornR) {
        glUniform1i(glGetUniformLocation(shaderProgram[1], "flip"), 1);
    } else {
        glUniform1i(glGetUniformLocation(shaderProgram[1], "flip"), 0);
    }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, layer->lines.tex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, layer->tint.tex);
    glBindVertexArray(models[4].vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawCreatorTroll(void) {
    glBindFramebuffer(GL_FRAMEBUFFER, creatorFB.fbo);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glUseProgram(shaderProgram[1]);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram[1], "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram[1], "projection"), 1, GL_FALSE, projection);
    mat4f_init(model);
    mat4f_trans_translate(model, 0.0, 0.0, -50.0);
    drawCreatorLayer(&bg);
    drawCreatorLayer(&acc_back);
    drawCreatorLayer(&hair);
    drawCreatorLayer(&body);
    drawCreatorLayer(&markings);
    drawCreatorLayer(&facepaint);
    drawCreatorLayer(&mouth);
    drawCreatorLayer(&eyebrows);
    drawCreatorLayer(&eyeL);
    drawCreatorLayer(&eyeR);
    drawCreatorLayer(&shoeL);
    drawCreatorLayer(&shoeR);
    drawCreatorLayer(&bottom);
    drawCreatorLayer(&acc_cloth);
    drawCreatorLayer(&upper);
    drawCreatorLayer(&sigil);
    drawCreatorLayer(&acc_face);
    drawCreatorLayer(&hornL);
    drawCreatorLayer(&hornR);
    drawCreatorLayer(&bangs);
    drawCreatorLayer(&acc_hats);
    glBindFramebuffer(GL_FRAMEBUFFER, screenFB.fbo);
}

void loadUniformsAndDrawTexturedOffset(modelinfo mdl, unsigned int shader, unsigned int texture, int offset, int size) {
    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, projection);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, model);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(mdl.vao);
    glDrawArrays(GL_TRIANGLES, offset, size);
}

void drawNumber(modelinfo mdl, unsigned int shader, unsigned int number, float scaleX, float scaleY) {
    float uOffset = number/16 * scaleX;
    float vOffset = (15 - number%16) * scaleY;
    glUniform1f(glGetUniformLocation(shader, "uOffset"), uOffset);
    glUniform1f(glGetUniformLocation(shader, "vOffset"), vOffset);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, model);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawNumbers(modelinfo mdl, unsigned int shader) {
    float scaleX = 32.0/mdl.texture.width;
    float scaleY = 32.0/mdl.texture.height;
    glUseProgram(shader);
    glUniform1f(glGetUniformLocation(shader, "scaleX"), scaleX);
    glUniform1f(glGetUniformLocation(shader, "scaleY"), scaleY);
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, projection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mdl.texture.tex);
    glBindVertexArray(mdl.vao);

    int assetNumbers[7];
    if (activeCategory == 0) {
        assetNumbers[0] = body.asset;
        assetNumbers[1] = facepaint.asset;
        assetNumbers[2] = markings.asset;
        assetNumbers[3] = mouth.asset;
        assetNumbers[4] = eyebrows.asset;
        assetNumbers[5] = eyeL.asset;
        assetNumbers[6] = eyeR.asset;
    } else if (activeCategory == 1) {
        assetNumbers[0] = sigil.asset;
        assetNumbers[1] = bangs.asset;
        assetNumbers[2] = hair.asset;
        assetNumbers[3] = upper.asset;
        assetNumbers[4] = bottom.asset;
        assetNumbers[5] = shoeL.asset;
        assetNumbers[6] = shoeR.asset;
    } else {
        assetNumbers[0] = acc_hats.asset;
        assetNumbers[1] = acc_cloth.asset;
        assetNumbers[2] = acc_face.asset;
        assetNumbers[3] = hornR.asset;
        assetNumbers[4] = hornL.asset;
        assetNumbers[5] = bg.asset;
        assetNumbers[6] = acc_back.asset;
    }

    mat4f_init(model);
    mat4f_trans_translate(model, 545.0, 324.0, -5.0);
    drawNumber(mdl, shader, assetNumbers[0], scaleX, scaleY);
    mat4f_trans_translate(model, 0.0, -96.0, 0.0);
    drawNumber(mdl, shader, assetNumbers[1], scaleX, scaleY);
    mat4f_trans_translate(model, -128.0, 0.0, 0.0);
    drawNumber(mdl, shader, assetNumbers[2], scaleX, scaleY);
    mat4f_trans_translate(model, 0.0, -96.0, 0.0);
    drawNumber(mdl, shader, assetNumbers[3], scaleX, scaleY);
    mat4f_trans_translate(model, 128.0, 0.0, 0.0);
    drawNumber(mdl, shader, assetNumbers[4], scaleX, scaleY);
    mat4f_trans_translate(model, 0.0, -96.0, 0.0);
    drawNumber(mdl, shader, assetNumbers[5], scaleX, scaleY);
    mat4f_trans_translate(model, -128.0, 0.0, 0.0);
    drawNumber(mdl, shader, assetNumbers[6], scaleX, scaleY);
}

void drawArrow(modelinfo mdl, unsigned int shader, bool direction, bool clicked, float scaleX, float scaleY) {
    // direction = 0: left
    // direction = 1: right
    float uOffset = (3*activeCategory + 3) * scaleX;
    float vOffset = 1.0 - (2*direction + clicked + 1.0) * scaleY;
    glUniform1f(glGetUniformLocation(shader, "uOffset"), uOffset);
    glUniform1f(glGetUniformLocation(shader, "vOffset"), vOffset);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, model);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawArrows(modelinfo mdl, unsigned int shader) {
    float scaleX = 32.0/mdl.texture.width;
    float scaleY = 32.0/mdl.texture.height;
    glUseProgram(shader);
    glUniform1f(glGetUniformLocation(shader, "scaleX"), scaleX);
    glUniform1f(glGetUniformLocation(shader, "scaleY"), scaleY);
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, projection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mdl.texture.tex);
    glBindVertexArray(mdl.vao);
    mat4f_init(model);
    mat4f_trans_translate(model, 511.0, 324.0, -5.0);
    drawArrow(mdl, shader, 0, buttonsClicked[9], scaleX, scaleY);
    mat4f_trans_translate(model, 64.0, 0.0, 0.0);
    drawArrow(mdl, shader, 1, buttonsClicked[11], scaleX, scaleY);
    mat4f_trans_translate(model, 0.0, -96.0, 0.0);
    drawArrow(mdl, shader, 1, buttonsClicked[23], scaleX, scaleY);
    mat4f_trans_translate(model, -64.0, 0.0, 0.0);
    drawArrow(mdl, shader, 0, buttonsClicked[21], scaleX, scaleY);
    mat4f_trans_translate(model, -128.0, 0.0, 0.0);
    drawArrow(mdl, shader, 0, buttonsClicked[15], scaleX, scaleY);
    mat4f_trans_translate(model, 64.0, 0.0, 0.0);
    drawArrow(mdl, shader, 1, buttonsClicked[17], scaleX, scaleY);
    mat4f_trans_translate(model, 0.0, -96.0, 0.0);
    drawArrow(mdl, shader, 1, buttonsClicked[29], scaleX, scaleY);
    mat4f_trans_translate(model, -64.0, 0.0, 0.0);
    drawArrow(mdl, shader, 0, buttonsClicked[27], scaleX, scaleY);
    mat4f_trans_translate(model, 128.0, 0.0, 0.0);
    drawArrow(mdl, shader, 0, buttonsClicked[33], scaleX, scaleY);
    mat4f_trans_translate(model, 64.0, 0.0, 0.0);
    drawArrow(mdl, shader, 1, buttonsClicked[35], scaleX, scaleY);
    mat4f_trans_translate(model, 0.0, -96.0, 0.0);
    drawArrow(mdl, shader, 1, buttonsClicked[47], scaleX, scaleY);
    mat4f_trans_translate(model, -64.0, 0.0, 0.0);
    drawArrow(mdl, shader, 0, buttonsClicked[45], scaleX, scaleY);
    mat4f_trans_translate(model, -128.0, 0.0, 0.0);
    drawArrow(mdl, shader, 0, buttonsClicked[39], scaleX, scaleY);
    mat4f_trans_translate(model, 64.0, 0.0, 0.0);
    drawArrow(mdl, shader, 1, buttonsClicked[41], scaleX, scaleY);
}

void drawCategory(modelinfo mdl, unsigned int shader, unsigned int category, float scaleX, float scaleY) {
    float uOffset = category * scaleX;
    float vOffset = category == activeCategory ? 1.0 - scaleY : 1.0 - 2 * scaleY;
    glUniform1f(glGetUniformLocation(shader, "uOffset"), uOffset);
    glUniform1f(glGetUniformLocation(shader, "vOffset"), vOffset);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, model);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawCategories(modelinfo mdl, unsigned int shader) {
    float scaleX = 64.0/mdl.texture.width;
    float scaleY = 64.0/mdl.texture.height;
    glUseProgram(shader);
    glUniform1f(glGetUniformLocation(shader, "scaleX"), scaleX);
    glUniform1f(glGetUniformLocation(shader, "scaleY"), scaleY);
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, projection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mdl.texture.tex);
    glBindVertexArray(mdl.vao);
    mat4f_init(model);
    mat4f_trans_translate(model, 400.0, 420.0, -5.0);
    drawCategory(mdl, shader, 0, scaleX, scaleY);
    mat4f_trans_translate(model, 64.0, 0.0, 0.0);
    drawCategory(mdl, shader, 1, scaleX, scaleY);
    mat4f_trans_translate(model, 64.0, 0.0, 0.0);
    drawCategory(mdl, shader, 2, scaleX, scaleY);
}

void drawCursor(modelinfo mdl, unsigned int shader) {
    float scaleX = 32.0/mdl.texture.width;
    float scaleY = 32.0/mdl.texture.height;
    float uOffset = 0.0;
    float vOffset = 1.0 - scaleY;
    glUseProgram(shader);
    glUniform1f(glGetUniformLocation(shader, "scaleX"), scaleX);
    glUniform1f(glGetUniformLocation(shader, "scaleY"), scaleY);
    glUniform1f(glGetUniformLocation(shader, "uOffset"), uOffset);
    glUniform1f(glGetUniformLocation(shader, "vOffset"), vOffset);
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, projection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mdl.texture.tex);
    glBindVertexArray(mdl.vao);
    mat4f_init(model);
    mat4f_trans_translate(model, cursor.x - 10.0, cursor.y - 24.0, 0.0);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, model);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawNumbersColors(modelinfo mdl, layertex layer, unsigned int shader) {
    float scaleX = 32.0/mdl.texture.width;
    float scaleY = 32.0/mdl.texture.height;
    glUseProgram(shader);
    glUniform1f(glGetUniformLocation(shader, "scaleX"), scaleX);
    glUniform1f(glGetUniformLocation(shader, "scaleY"), scaleY);
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, projection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mdl.texture.tex);
    glBindVertexArray(mdl.vao);

    float num1, num2, num3;
    num1 = num2 = num3 = 0;
    if (activeColorMode != 1) {
        num1 = roundf((float)layer.r * 255);
        num2 = roundf((float)layer.g * 255);
        num3 = roundf((float)layer.b * 255);
        sliderX[0] = layer.r * 480 + 66;
        sliderX[1] = layer.g * 480 + 66;
        sliderX[2] = layer.b * 480 + 66;
    } else {
        RGBtoHSL(&num1, &num2, &num3, layer.r, layer.g, layer.b);
        sliderX[0] = num1 * 480 + 66;
        sliderX[1] = num2 * 480 + 66;
        sliderX[2] = num3 * 480 + 66;
        num1 = roundf((float)num1*360);
        num2 = roundf((float)num2*100);
        num3 = roundf((float)num3*100);
    }
    if (activeColorMode == 2) {
        int hex1 = ((int)num1 / 16) + 368;
        int hex2 = ((int)num1 % 16) + 368;
        int hex3 = ((int)num2 / 16) + 368;
        int hex4 = ((int)num2 % 16) + 368;
        int hex5 = ((int)num3 / 16) + 368;
        int hex6 = ((int)num3 % 16) + 368;
        mat4f_init(model);
        mat4f_trans_translate(model, 225.0, 68.0, -5.0);
        drawNumber(mdl, shader, hex1, scaleX, scaleY);
        mat4f_trans_translate(model, 32.0, 0.0, 0.0);
        drawNumber(mdl, shader, hex2, scaleX, scaleY);
        mat4f_trans_translate(model, 32.0, 0.0, 0.0);
        drawNumber(mdl, shader, hex3, scaleX, scaleY);
        mat4f_trans_translate(model, 32.0, 0.0, 0.0);
        drawNumber(mdl, shader, hex4, scaleX, scaleY);
        mat4f_trans_translate(model, 32.0, 0.0, 0.0);
        drawNumber(mdl, shader, hex5, scaleX, scaleY);
        mat4f_trans_translate(model, 32.0, 0.0, 0.0);
        drawNumber(mdl, shader, hex6, scaleX, scaleY);
    } else {
        mat4f_init(model);
        mat4f_trans_translate(model, 577.0, 100.0, -5.0);
        drawNumber(mdl, shader, num1, scaleX, scaleY);
        mat4f_trans_translate(model, 0.0, -32.0, 0.0);
        drawNumber(mdl, shader, num2, scaleX, scaleY);
        mat4f_trans_translate(model, 0.0, -32.0, 0.0);
        drawNumber(mdl, shader, num3, scaleX, scaleY);
    }
}

void drawArrowColors(modelinfo mdl, unsigned int shader, bool hex, bool direction, bool clicked, float scaleX, float scaleY) {
    // direction = 0: up
    // direction = 1: down
    float uOffset = (3 * hex + direction) * scaleX;
    float vOffset = 1.0 - (5 + clicked) * scaleY;
    glUniform1f(glGetUniformLocation(shader, "uOffset"), uOffset);
    glUniform1f(glGetUniformLocation(shader, "vOffset"), vOffset);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, model);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawArrowsColors(modelinfo mdl, unsigned int shader) {
    float scaleX = 32.0/mdl.texture.width;
    float scaleY = 32.0/mdl.texture.height;
    glUseProgram(shader);
    glUniform1f(glGetUniformLocation(shader, "scaleX"), scaleX);
    glUniform1f(glGetUniformLocation(shader, "scaleY"), scaleY);
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, projection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mdl.texture.tex);
    glBindVertexArray(mdl.vao);
    mat4f_init(model);
    if (activeColorMode != 2) {
        mat4f_trans_translate(model, sliderX[0], 96.0, -5.0);
        drawArrowColors(mdl, shader, 0, 0, 0, scaleX, scaleY);
        mat4f_init(model);
        mat4f_trans_translate(model, sliderX[1], 64.0, -5.0);
        drawArrowColors(mdl, shader, 0, 0, 0, scaleX, scaleY);
        mat4f_init(model);
        mat4f_trans_translate(model, sliderX[2], 32.0, -5.0);
        drawArrowColors(mdl, shader, 0, 0, 0, scaleX, scaleY);
    } else {
        mat4f_trans_translate(model, 224.0, 100.0, -5.0);
        drawArrowColors(mdl, shader, 1, 0, buttonsClicked[45], scaleX, scaleY);
        mat4f_trans_translate(model, 32.0, 0.0, 0.0);
        drawArrowColors(mdl, shader, 1, 0, buttonsClicked[46], scaleX, scaleY);
        mat4f_trans_translate(model, 32.0, 0.0, 0.0);
        drawArrowColors(mdl, shader, 1, 0, buttonsClicked[47], scaleX, scaleY);
        mat4f_trans_translate(model, 32.0, 0.0, 0.0);
        drawArrowColors(mdl, shader, 1, 0, buttonsClicked[48], scaleX, scaleY);
        mat4f_trans_translate(model, 32.0, 0.0, 0.0);
        drawArrowColors(mdl, shader, 1, 0, buttonsClicked[49], scaleX, scaleY);
        mat4f_trans_translate(model, 32.0, 0.0, 0.0);
        drawArrowColors(mdl, shader, 1, 0, buttonsClicked[50], scaleX, scaleY);
        mat4f_trans_translate(model, -160.0, -64.0, 0.0);
        drawArrowColors(mdl, shader, 1, 1, buttonsClicked[51], scaleX, scaleY);
        mat4f_trans_translate(model, 32.0, 0.0, 0.0);
        drawArrowColors(mdl, shader, 1, 1, buttonsClicked[52], scaleX, scaleY);
        mat4f_trans_translate(model, 32.0, 0.0, 0.0);
        drawArrowColors(mdl, shader, 1, 1, buttonsClicked[53], scaleX, scaleY);
        mat4f_trans_translate(model, 32.0, 0.0, 0.0);
        drawArrowColors(mdl, shader, 1, 1, buttonsClicked[54], scaleX, scaleY);
        mat4f_trans_translate(model, 32.0, 0.0, 0.0);
        drawArrowColors(mdl, shader, 1, 1, buttonsClicked[55], scaleX, scaleY);
        mat4f_trans_translate(model, 32.0, 0.0, 0.0);
        drawArrowColors(mdl, shader, 1, 1, buttonsClicked[56], scaleX, scaleY);
    }
}