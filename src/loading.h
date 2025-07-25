#pragma once

#include "program.h"

unsigned int moduint(int x, int y);
float moduf(float x, float y);
unsigned int shaderLoad(const char* vsPath, const char* fsPath);
void textureLoad(textureinfo *texture, const char* texPath, unsigned int format);
unsigned int modelLoad(modelinfo *dest, const char* modelPath);
void loadIntoVAO(modelinfo *model);
void createFramebuffer(framebuffer *fb, int width, int height, int offsetX);
void createRectFromTexture(modelinfo *mdl, textureinfo texture);
void modifyUIWindow(modelinfo *mdl, unsigned int width, unsigned int height);
void modifySelectionOverlay(modelinfo *mdl, unsigned int width, unsigned int height);
void loadCreatorTexture(layertex *layer, int asset, float r, float g, float b);
void loadCreatorTextureNoColor(layertex *layer, int asset);
void exportTrollImage(void);
void RGBtoHSL(float *h, float *s, float *l, float r, float g, float b);
void HSLtoRGB(float *r, float *g, float *b, float h, float s, float l);
void loadTitleScreen(void);
void loadCreator(void);
void loadColors(void);