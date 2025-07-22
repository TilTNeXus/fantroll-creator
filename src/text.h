#pragma once
#include "program.h"

void loadFont(const char *fontpath);
void drawText(unsigned int shader, char *text, float x, float y, float scale, float color[3]);