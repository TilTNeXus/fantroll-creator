#pragma once

#include "program.h"

unsigned int moduint(int x, int y);
float moduf(float x, float y);
unsigned int shaderLoad(const char* vsPath, const char* fsPath);
void textureLoad(textureinfo *texture, const char* texPath, unsigned int format);
unsigned int modelLoad(modelinfo *dest, const char* modelPath);
void loadIntoVAO(modelinfo *model);
void createFramebuffer(framebuffer *fb, int width, int height, int offsetX);
void loadUniformsAndDraw(modelinfo mdl, unsigned int shader);
void loadUniformsAndDrawTextured(modelinfo mdl, unsigned int shader, unsigned int texture);
void createRectFromTexture(modelinfo *mdl, textureinfo texture);
void modifyUIWindow(modelinfo *mdl, unsigned int width, unsigned int height);
void modifySelectionOverlay(modelinfo *mdl, unsigned int width, unsigned int height);
void loadCreatorTexture(layertex *layer, int asset, float r, float g, float b);
void loadCreatorTextureNoColor(layertex *layer, int asset);
void drawCreatorLayer(layertex *layer);
void drawCreatorTroll(void);
void loadUniformsAndDrawTexturedOffset(modelinfo mdl, unsigned int shader, unsigned int texture, int offset, int size);
void drawNumbers(modelinfo mdl, unsigned int shader);
void drawArrows(modelinfo mdl, unsigned int shader);
void drawCategories(modelinfo mdl, unsigned int shader);
void drawCursor(modelinfo mdl, unsigned int shader);
void exportTrollImage(void);
void RGBtoHSL(float *h, float *s, float *l, float r, float g, float b);
void drawNumbersColors(modelinfo mdl, layertex layer, unsigned int shader);