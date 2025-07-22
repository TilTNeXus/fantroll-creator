#include "mouse.h"
#include "loading.h"

bool mouseInBox(mouseState ms, int left, int right, int bottom, int top) {
    return left <= ms.x && ms.x <= right && bottom <= ms.y && ms.y <= top;
}

int detectUIButtons(mouseState ms) {
    int catsX;
    int catsY = 420;
    int catsSize = 64;
    for (int i = 0; i < 3; i++) {
        catsX = 400 + i*catsSize;
        if (mouseInBox(ms, catsX, catsX + catsSize, catsY, catsY + catsSize)) {
            activeCategory = i;
            char categoryTilesetPath[32];
            snprintf(categoryTilesetPath, 32, "assets/tilesets/Doll%d_A2.png", i+1);
            glDeleteTextures(1, &models[3].texture.tex);
            textureLoad(&models[3].texture, categoryTilesetPath, GL_RGBA);
        }
    }

    int buttonsX, buttonsY;
    int buttonsSize = 32;
    int clickedButton = -1;
    for (int i = 0; i < 48; i++) {
        buttonsX = 384 + buttonsSize*(i%3) + 4*buttonsSize*((i/6)%2);
        buttonsY = 356 - buttonsSize*((i/3)%2) - 3*buttonsSize*(i/12);
        if (mouseInBox(ms, buttonsX, buttonsX+buttonsSize, buttonsY, buttonsY+buttonsSize)) {
            clickedButton = i;
            break;
        } else {
            clickedButton = -1;
        }
    }
    buttonsClicked[clickedButton] = 1;
    return clickedButton;
}
void updateClickedButtons(int clicked) {
    int partAsset;
    switch (clicked) {
        case -1:
            break;
        case 3:
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
        case 9:
            if (activeCategory == 0) {
                partAsset = moduint(body.asset-2, AMT_BODY)+1;
                loadCreatorTextureNoColor(&body, partAsset);
            } else if (activeCategory == 1) {
                partAsset = moduint(sigil.asset-1, AMT_SIGIL);
                loadCreatorTextureNoColor(&sigil, partAsset);
            } else {
                partAsset = moduint(acc_hats.asset-1, AMT_ACC_HATS);
                loadCreatorTextureNoColor(&acc_hats, partAsset);
            }
            break;
        case 11:
            if (activeCategory == 0) {
                partAsset = (body.asset % AMT_BODY)+1;
                loadCreatorTextureNoColor(&body, partAsset);
            } else if (activeCategory == 1) {
                partAsset = (sigil.asset+1) % AMT_SIGIL;
                loadCreatorTextureNoColor(&sigil, partAsset);
            } else {
                partAsset = (acc_hats.asset+1) % AMT_ACC_HATS;
                loadCreatorTextureNoColor(&acc_hats, partAsset);
            }
            break;
        case 15:
            if (activeCategory == 0) {
                partAsset = moduint(markings.asset-1, AMT_MARKINGS);
                loadCreatorTextureNoColor(&markings, partAsset);
            } else if (activeCategory == 1) {
                partAsset = moduint(hair.asset-1, AMT_HAIR);
                loadCreatorTextureNoColor(&hair, partAsset);
            } else {
                partAsset = moduint(acc_face.asset-1, AMT_ACC_FACE);
                loadCreatorTextureNoColor(&acc_face, partAsset);
            }
            break;
        case 17:
            if (activeCategory == 0) {
                partAsset = (markings.asset+1) % AMT_MARKINGS;
                loadCreatorTextureNoColor(&markings, partAsset);
            } else if (activeCategory == 1) {
                partAsset = (hair.asset+1) % AMT_HAIR;
                loadCreatorTextureNoColor(&hair, partAsset);
            } else {
                partAsset = (acc_face.asset+1) % AMT_ACC_FACE;
                loadCreatorTextureNoColor(&acc_face, partAsset);
            }
            break;
        case 21:
            if (activeCategory == 0) {
                partAsset = moduint(facepaint.asset-1, AMT_FACEPAINT);
                loadCreatorTextureNoColor(&facepaint, partAsset);
            } else if (activeCategory == 1) {
                partAsset = moduint(bangs.asset-1, AMT_BANGS);
                loadCreatorTextureNoColor(&bangs, partAsset);
            } else {
                partAsset = moduint(acc_cloth.asset-1, AMT_ACC_CLOTH);
                loadCreatorTextureNoColor(&acc_cloth, partAsset);
            }
            break;
        case 23:
            if (activeCategory == 0) {
                partAsset = (facepaint.asset+1) % AMT_FACEPAINT;
                loadCreatorTextureNoColor(&facepaint, partAsset);
            } else if (activeCategory == 1) {
                partAsset = (bangs.asset+1) % AMT_BANGS;
                loadCreatorTextureNoColor(&bangs, partAsset);
            } else {
                partAsset = (acc_cloth.asset+1) % AMT_ACC_CLOTH;
                loadCreatorTextureNoColor(&acc_cloth, partAsset);
            }
            break;
        case 27:
            if (activeCategory == 0) {
                partAsset = moduint(mouth.asset-1, AMT_MOUTH);
                loadCreatorTextureNoColor(&mouth, partAsset);
            } else if (activeCategory == 1) {
                partAsset = moduint(upper.asset-1, AMT_UPPER);
                loadCreatorTextureNoColor(&upper, partAsset);
            } else {
                partAsset = moduint(hornR.asset-1, AMT_HORN);
                loadCreatorTextureNoColor(&hornR, partAsset);
            }
            break;
        case 29:
            if (activeCategory == 0) {
                partAsset = (mouth.asset+1) % AMT_MOUTH;
                loadCreatorTextureNoColor(&mouth, partAsset);
            } else if (activeCategory == 1) {
                partAsset = (upper.asset+1) % AMT_UPPER;
                loadCreatorTextureNoColor(&upper, partAsset);
            } else {
                partAsset = (hornR.asset+1) % AMT_HORN;
                loadCreatorTextureNoColor(&hornR, partAsset);
            }
            break;
        case 33:
            if (activeCategory == 0) {
                partAsset = moduint(eyebrows.asset-1, AMT_EYEBROWS);
                loadCreatorTextureNoColor(&eyebrows, partAsset);
            } else if (activeCategory == 1) {
                partAsset = moduint(bottom.asset-1, AMT_BOTTOM);
                loadCreatorTextureNoColor(&bottom, partAsset);
            } else {
                partAsset = moduint(hornL.asset-1, AMT_HORN);
                loadCreatorTextureNoColor(&hornL, partAsset);
            }
            break;
        case 35:
            if (activeCategory == 0) {
                partAsset = (eyebrows.asset+1) % AMT_EYEBROWS;
                loadCreatorTextureNoColor(&eyebrows, partAsset);
            } else if (activeCategory == 1) {
                partAsset = (bottom.asset+1) % AMT_BOTTOM;
                loadCreatorTextureNoColor(&bottom, partAsset);
            } else {
                partAsset = (hornL.asset+1) % AMT_HORN;
                loadCreatorTextureNoColor(&hornL, partAsset);
            }
            break;
        case 39:
            if (activeCategory == 0) {
                partAsset = moduint(eyeR.asset-1, AMT_EYE);
                loadCreatorTextureNoColor(&eyeR, partAsset);
            } else if (activeCategory == 1) {
                partAsset = moduint(shoeR.asset-1, AMT_SHOE);
                loadCreatorTextureNoColor(&shoeR, partAsset);
            } else {
                partAsset = moduint(acc_back.asset-1, AMT_ACC_BACK);
                loadCreatorTextureNoColor(&acc_back, partAsset);
            }
            break;
        case 41:
            if (activeCategory == 0) {
                partAsset = (eyeR.asset+1) % AMT_EYE;
                loadCreatorTextureNoColor(&eyeR, partAsset);
            } else if (activeCategory == 1) {
                partAsset = (shoeR.asset+1) % AMT_SHOE;
                loadCreatorTextureNoColor(&shoeR, partAsset);
            } else {
                partAsset = (acc_back.asset+1) % AMT_ACC_BACK;
                loadCreatorTextureNoColor(&acc_back, partAsset);
            }
            break;
        case 45:
            if (activeCategory == 0) {
                partAsset = moduint(eyeL.asset-1, AMT_EYE);
                loadCreatorTextureNoColor(&eyeL, partAsset);
            } else if (activeCategory == 1) {
                partAsset = moduint(shoeL.asset-1, AMT_SHOE);
                loadCreatorTextureNoColor(&shoeL, partAsset);
            } else {
                partAsset = moduint(bg.asset-1, AMT_BG);
                loadCreatorTextureNoColor(&bg, partAsset);
            }
            break;
        case 47:
            if (activeCategory == 0) {
                partAsset = (eyeL.asset+1) % AMT_EYE;
                loadCreatorTextureNoColor(&eyeL, partAsset);
            } else if (activeCategory == 1) {
                partAsset = (shoeL.asset+1) % AMT_SHOE;
                loadCreatorTextureNoColor(&shoeL, partAsset);
            } else {
                partAsset = (bg.asset+1) % AMT_BG;
                loadCreatorTextureNoColor(&bg, partAsset);
            }
            break;
    }
}

int detectUIButtonsColor(mouseState ms) {
    int presetsX, presetsY;
    int buttonsSize = 32;
    int clickedButton = -1;
    for (int i = 0; i < 45; i++) {
        presetsX = 289 + buttonsSize*(i%9);
        presetsY = 389 - buttonsSize*((i/9));
        if (mouseInBox(ms, presetsX, presetsX+29, presetsY, presetsY+30)) {
            clickedButton = i;
            break;
        } else {
            clickedButton = -1;
        }
    }
    buttonsClicked[clickedButton] = 1;
    return clickedButton;
}
void updateClickedButtonsColor(layertex *layer, int clicked) {
    int noChange = 0;
    int r, g, b;
    switch (clicked) {
        case 0: // rust
            r = 0xa1;
            g = 0;
            b = 0;
            break;
        case 1: // bronze
            r = 0xa2;
            g = 0x52;
            b = 0x03;
            break;
        case 2: // gold
            r = 0xa1;
            g = 0xa1;
            b = 0x00;
            break;
        case 4: // breath bg
            r = 0x2d;
            g = 0x7b;
            b = 0xee;
            break;
        case 5: // breath fg
            r = 0x00;
            g = 0xe3;
            b = 0xfd;
            break;
        case 6: // heart bg
            r = 0x5d;
            g = 0x0b;
            b = 0x2a;
            break;
        case 7: // heart fg
            r = 0xcf;
            g = 0x00;
            b = 0x65;
            break;
        case 8: // white
            r = 0xff;
            g = 0xff;
            b = 0xff;
            break;
        case 9: // lime
            r = 0x65;
            g = 0x82;
            b = 0x00;
            break;
        case 10: // olive
            r = 0x41;
            g = 0x66;
            b = 0x00;
            break;
        case 11: // jade
            r = 0x07;
            g = 0x84;
            b = 0x46;
            break;
        case 13: // light bg
            r = 0xff;
            g = 0x7d;
            b = 0x00;
            break;
        case 14: // light fg
            r = 0xf5;
            g = 0xfa;
            b = 0x00;
            break;
        case 15: // life bg
            r = 0xa7;
            g = 0x96;
            b = 0x85;
            break;
        case 16: // life fg
            r = 0x26;
            g = 0xee;
            b = 0x00;
            break;
        case 17: // light gray
            r = 0xc4;
            g = 0xc4;
            b = 0xc4;
            break;
        case 18: // teal
            r = 0x00;
            g = 0x82;
            b = 0x82;
            break;
        case 19: // blue
            r = 0x00;
            g = 0x41;
            b = 0x82;
            break;
        case 20: // indigo
            r = 0x00;
            g = 0x21;
            b = 0xcb;
            break;
        case 22: // time bg
            r = 0xc8;
            g = 0x00;
            b = 0x00;
            break;
        case 23: // time fg
            r = 0xff;
            g = 0x00;
            b = 0x00;
            break;
        case 24: // blood bg
            r = 0x44;
            g = 0x13;
            b = 0x00;
            break;
        case 25: // blood fg
            r = 0xcb;
            g = 0x00;
            b = 0x00;
            break;
        case 26: // medium gray
            r = 0x98;
            g = 0x98;
            b = 0x98;
            break;
        case 27: // purple
            r = 0x44;
            g = 0x0a;
            b = 0x7f;
            break;
        case 28: // violet
            r = 0x6a;
            g = 0x00;
            b = 0x6a;
            break;
        case 29: // fuchsia
            r = 0x99;
            g = 0x00;
            b = 0x4d;
            break;
        case 31: // mind bg
            r = 0x00;
            g = 0x95;
            b = 0x2f;
            break;
        case 32: // mind fg
            r = 0x00;
            g = 0xff;
            b = 0xc6;
            break;
        case 33: // hope
            r = 0xff;
            g = 0xdc;
            b = 0x1a;
            break;
        case 34: // doom
            r = 0x10;
            g = 0x6a;
            b = 0x00;
            break;
        case 35: // dark gray
            r = 0x6c;
            g = 0x6c;
            b = 0x6c;
            break;
        case 36: // horn 1
            r = 0xff;
            g = 0x42;
            b = 0x01;
            break;
        case 37: // horn 2
            r = 0xff;
            g = 0x91;
            b = 0x00;
            break;
        case 38: // horn 3
            r = 0xff;
            g = 0xba;
            b = 0x28;
            break;
        case 40: // void fg
            r = 0x00;
            g = 0x1b;
            b = 0x5c;
            break;
        case 41: // void bg
            r = 0x00;
            g = 0x4f;
            b = 0xa8;
            break;
        case 42: // rage bg
            r = 0x5a;
            g = 0x01;
            b = 0x65;
            break;
        case 43: // rage fg
            r = 0xa8;
            g = 0x47;
            b = 0xb2;
            break;
        case 44: // black
            r = 0x00;
            g = 0x00;
            b = 0x00;
            break;
        default:
            noChange = 1;
            break;
    }
    if (!noChange) {
            layer->r = (float)r/255;
            layer->g = (float)g/255;
            layer->b = (float)b/255;
        }
}