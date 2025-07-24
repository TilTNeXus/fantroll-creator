#pragma once

#include "program.h"

bool mouseInBox(mouseState ms, int left, int right, int bottom, int top);
int detectUIButtons(mouseState ms);
void updateClickedButtons(int clicked);
int detectUIButtonsColor(mouseState ms);
void updateClickedButtonsColor(layertex *layer, int clicked);
void updateSlidersColor(mouseState ms, layertex *layer);