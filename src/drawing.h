#pragma once

#include "program.h"

void loadUniformsAndDraw(modelinfo mdl, unsigned int shader);
void loadUniformsAndDrawTextured(modelinfo mdl, unsigned int shader, unsigned int texture);
void drawCreatorLayer(layertex *layer);
void drawCreatorTroll(void);
void loadUniformsAndDrawTexturedOffset(modelinfo mdl, unsigned int shader, unsigned int texture, int offset, int size);
void drawNumbers(modelinfo mdl, unsigned int shader);
void drawArrows(modelinfo mdl, unsigned int shader);
void drawCategories(modelinfo mdl, unsigned int shader);
void drawCursor(modelinfo mdl, unsigned int shader);
void drawNumbersColors(modelinfo mdl, layertex layer, unsigned int shader);
void drawArrowsColors(modelinfo mdl, unsigned int shader);