#include "loading.h"
#include "time.h"

unsigned int moduint(int x, int y) {
    return ((x) % y + y) % y;
}
float moduf(float x, float y) {
    return fmod(fmod(x, y) + y, y);
}

unsigned int shaderLoad(const char* vsPath, const char* fsPath) {
    FILE *vsFile;
    FILE *fsFile;
    long int vsLength;
    long int fsLength;
    char *vsContent;
    char *fsContent;
    unsigned int vs, fs, program;
    int compiled;
    
    vsFile = fopen(vsPath, "r");
    if (!vsFile) {
        printf("%s does not exist\n", vsPath);
        return 0;
    } 
    fseek(vsFile, 0, SEEK_END);
    vsLength = ftell(vsFile) + 4;
    fseek(vsFile, 0, SEEK_SET);
    vsContent = (char*)malloc((vsLength+1)*sizeof(char));
    char line[256];
    vsContent[0] = '\0';
    while (fgets(line, 256, vsFile)) {
        if (strstr(line, "#version") && isGLES) {
            strcat(vsContent, "#version 300 es\n");
        } else {
            strcat(vsContent, line);
        }
    }
    vsContent[vsLength] = '\0';
    fclose(vsFile);
    const char* vsSource = vsContent;

    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vsSource, NULL);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &compiled);
    if (compiled) printf("compiled %s\n", vsPath);
    else printf("failed compiling %s\n", vsPath);

    free(vsContent);


    fsFile = fopen(fsPath, "r");
    if (!fsFile) {
        printf("%s does not exist\n", fsPath);
        return 0;
    }
    fseek(fsFile, 0, SEEK_END);
    fsLength = ftell(fsFile) + 4;
    fseek(fsFile, 0, SEEK_SET);
    fsContent = (char*)malloc((fsLength+1)*sizeof(char));
    fsContent[0] = '\0';
    while (fgets(line, 256, fsFile)) {
        if (strstr(line, "#version") && isGLES) {
            strcat(fsContent, "#version 300 es\n");
        } else {
            strcat(fsContent, line);
        }
    }
    fsContent[fsLength] = '\0';
    fclose(fsFile);
    const char* fsSource = fsContent;

    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fsSource, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &compiled);
    if (compiled) printf("compiled %s\n", fsPath);
    else {
        printf("failed compiling %s\n", fsPath);
        char buffer[256];
        int loglength;
        glGetShaderInfoLog(fs, 256, &loglength, buffer);
        printf("%s\n", buffer);
    }

    free(fsContent);

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void textureLoad(textureinfo *texture, const char* texPath, unsigned int format) {
    unsigned int t;
    int width, height, channels;
    glGenTextures(1, &t);
    glBindTexture(GL_TEXTURE_2D, t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    stbi_set_flip_vertically_on_load(true);
    unsigned char *imageData = stbi_load(texPath, &width, &height, &channels, 0);
    if (imageData) {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
    } else {
        printf("failed to load %s\n", texPath);
    }
    stbi_image_free(imageData);
    texture->tex = t;
    texture->width = width;
    texture->height = height;
}

unsigned int modelLoad(modelinfo *dest, const char* modelPath) {
    unsigned int vcount, vtcount, vncount, fcount;
    vcount = vtcount = vncount = fcount = 0;
    size_t vsize, vtsize, vnsize, vertssize;
    vsize = vtsize = vnsize = vertssize = 1024;
    FILE *modelFile;
    
    modelFile = fopen(modelPath, "r");
    if (!modelFile) {
        printf("%s does not exist\n", modelPath);
        return 0;
    } 

    char line[256];
    char *token;
    float *v = malloc(vsize*sizeof(float));
    float *vt = malloc(vtsize*sizeof(float));
    float *vn = malloc(vnsize*sizeof(float));
    float *verts = malloc(vertssize*sizeof(float));

    while (fgets(line, sizeof(line), modelFile)) {
        if (strncmp(line, "v ", 2) == 0) {
            token = strtok(line, " ");
            token = strtok(NULL, " ");
            if (vsize < vcount+3) {
                vsize *= 2;
                float *tmp = realloc(v, vsize*sizeof(float));
                v = tmp;
            }
            while (token) {
                v[vcount] = atof(token);
                token = strtok(NULL, " ");
                vcount++;
            }
        } else if (strncmp(line, "vt", 2) == 0) {
            token = strtok(line, " ");
            token = strtok(NULL, " ");
            if (vtsize < vtcount+3) {
                vtsize *= 2;
                float *tmp = realloc(vt, vtsize*sizeof(float));
                vt = tmp;
            }
            while (token) {
                vt[vtcount] = atof(token);
                token = strtok(NULL, " ");
                vtcount++;
            }
        } else if (strncmp(line, "vn", 2) == 0) {
            token = strtok(line, " ");
            token = strtok(NULL, " ");
            if (vnsize < vncount+3) {
                vnsize *= 2;
                float *tmp = realloc(vn, vnsize*sizeof(float));
                vn = tmp;
            }
            while (token) {
                vn[vncount] = atof(token);
                token = strtok(NULL, " ");
                vncount++;
            }
        } else if (strncmp(line, "f ", 2) == 0) {
            char *saveOuter;
            token = strtok_r(line, " ", &saveOuter);
            token = strtok_r(NULL, " ", &saveOuter);
            while (token) {
                int vi, vti, vni;
                if (vertssize < fcount*8+8) {
                    vertssize *= 2;
                    float *tmp = realloc(verts, vertssize*sizeof(float));
                    verts = tmp;
                }
                sscanf(token, "%d/%d/%d", &vi, &vti, &vni);
                vi--;
                vti--;
                vni--;
                verts[8*fcount] = v[3*vi];
                verts[8*fcount+1] = v[3*vi+1];
                verts[8*fcount+2] = v[3*vi+2];
                verts[8*fcount+3] = vt[2*vti];
                verts[8*fcount+4] = vt[2*vti+1];
                verts[8*fcount+5] = vn[3*vni];
                verts[8*fcount+6] = vn[3*vni+1];
                verts[8*fcount+7] = vn[3*vni+2];

                fcount++;
                token = strtok_r(NULL, " ", &saveOuter);                
            }
        }
    }

    free(v);
    free(vt);
    free(vn);
    fclose(modelFile);
    dest->verts = verts;
    dest->size = fcount;
    //printf("loaded %s\n", modelPath);

    return 1;
}
void loadIntoVAO(modelinfo *model) {
    glGenVertexArrays(1, &model->vao);
    glBindVertexArray(model->vao);
    glGenBuffers(1, &model->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, model->vbo);
    glBufferData(GL_ARRAY_BUFFER, model->size*8*sizeof(float), model->verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(5*sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    free(model->verts);
}

void createFramebuffer(framebuffer *fb, int width, int height, int offsetX) {
    glGenFramebuffers(1, &fb->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fb->fbo);
    glGenTextures(1, &fb->tex);
    glBindTexture(GL_TEXTURE_2D, fb->tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb->tex, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenRenderbuffers(1, &fb->rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, fb->rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fb->rbo);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    float right = (float)width/SCREEN_WIDTH;
    float top = (float)height/SCREEN_HEIGHT;
    float ox = (float)offsetX/SCREEN_WIDTH;
    float rect[] = {
        -right + ox,  top, 0.0f,  0.0f, 1.0f,
        -right + ox, -top, 0.0f,  0.0f, 0.0f,
        right + ox, -top, 0.0f,  1.0f, 0.0f,
        -right + ox,  top, 0.0f,  0.0f, 1.0f,
        right + ox, -top, 0.0f,  1.0f, 0.0f,
        right + ox,  top, 0.0f,  1.0f, 1.0f
    };
    glGenVertexArrays(1, &fb->vao);
    glBindVertexArray(fb->vao);
    glGenBuffers(1, &fb->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, fb->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    fb->shader = shaderLoad("assets/shaders/framebuffer.vs", "assets/shaders/framebuffer.fs");
}

void createRectFromTexture(modelinfo *mdl, textureinfo texture) {
    float *verts = malloc(48*sizeof(float));
    verts[0] = verts[24] = verts[32] = (float)texture.width;
    verts[9] = verts[33] = verts[41] = (float)texture.height;
    verts[1] = verts[2] = verts[4] = verts[5] = verts[6] = verts[8] = verts[10] = verts[11] = verts[13] = verts[14] = verts[16] = verts[17] = verts[18] = verts[19] = verts[20] = verts[21] = verts[22] = verts[25] = verts[26] = verts[28] = verts[29] = verts[30] = verts[34] = verts[37] = verts[38] = verts[40] = verts[42] = verts[43] = verts[45] = verts[46] = 0.0;
    verts[3] = verts[7] = verts[12] = verts[15] = verts[23] = verts[27] = verts[31] = verts[35] = verts[36] = verts[39] = verts[44] = verts[47] = 1.0;
    mdl->verts = verts;
    mdl->size = 6;
}

void modifyUIWindow(modelinfo *mdl, unsigned int width, unsigned int height) {
    int widthInObj = 64;
    int heightInObj = 32;
    for (int i = 0; i < 8*mdl->size; i++) {
        if ((int)mdl->verts[i] == widthInObj) {
            mdl->verts[i] = (float)(width);
        } else if ((int)mdl->verts[i] == widthInObj - 8) {
            mdl->verts[i] = (float)(width - 8);
        } else if ((int)mdl->verts[i] == widthInObj - 7) {
            mdl->verts[i] = (float)(width - 7);
        } else if ((int)mdl->verts[i] == heightInObj) {
            mdl->verts[i] = (float)(height);
        } else if ((int)mdl->verts[i] == heightInObj - 8) {
            mdl->verts[i] = (float)(height - 8);
        } else if ((int)mdl->verts[i] == heightInObj - 7) {
            mdl->verts[i] = (float)(height - 7);
        } else if ((int)mdl->verts[i] == heightInObj - 6) {
            mdl->verts[i] = (float)(height - 6);
        }
    }
}

void modifySelectionOverlay(modelinfo *mdl, unsigned int width, unsigned int height) {
    int widthInObj = 64;
    int heightInObj = 32;
    for (int i = 0; i < 8*mdl->size; i++) {
        if ((int)mdl->verts[i] == widthInObj) {
            mdl->verts[i] = (float)(width);
        } else if ((int)mdl->verts[i] == widthInObj - 3) {
            mdl->verts[i] = (float)(width - 3);
        } else if ((int)mdl->verts[i] == heightInObj) {
            mdl->verts[i] = (float)(height);
        } else if ((int)mdl->verts[i] == heightInObj - 3) {
            mdl->verts[i] = (float)(height - 3);
        }
    }
}

void loadCreatorTexture(layertex *layer, int asset, float r, float g, float b) {
    glDeleteTextures(1, &layer->lines.tex);
    glDeleteTextures(1, &layer->tint.tex);
    char imgName[40];
    if ((strcmp(layer->layername, "sigil") && asset != 0) || !strcmp(layer->layername, "bg")) {
        snprintf(imgName, 40, "assets/layers/%s (%d).png", layer->layername, asset);
        textureLoad(&layer->lines, imgName, GL_RGBA);
    } else {
        textureLoad(&layer->lines, "assets/layers/transparent.png", GL_RGBA);
    }

    if (strcmp(layer->layername, "bg") && strcmp(layer->layername, "markings") && strcmp(layer->layername, "facepaint") && strcmp(layer->layername, "eyebrows") && strcmp(layer->layername, "horn") && asset != 0) {
        snprintf(imgName, 40, "assets/layers/%sTint (%d).png", layer->layername, asset);
        textureLoad(&layer->tint, imgName, GL_RGBA);
    } else {
        textureLoad(&layer->tint, "assets/layers/transparent.png", GL_RGBA);
    }
    layer->asset = asset;
    layer->r = r;
    layer->g = g;
    layer->b = b;
}

void loadCreatorTextureNoColor(layertex *layer, int asset) {
    loadCreatorTexture(layer, asset, layer->r, layer->g, layer->b);
}

void exportTrollImage(void) {
    glBindFramebuffer(GL_FRAMEBUFFER, creatorFB.fbo);
    int width = 288;
    int height = 416;
    unsigned char *troll = malloc(4*width*height);
    unsigned char *flipped = malloc(4*width*height);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, troll);
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            for (int channel = 0; channel < 4; channel++) {
                flipped[(row*width + col)*4 + channel] = troll[((height-row-1)*width + col)*4 + channel];
            }
        }
    }
    struct tm *currentTime;
    time_t t;
    t = time(NULL);
    currentTime = localtime(&t);
    char exportFile[64];
    strftime(exportFile, 64, "Troll %Y-%m-%d %H%M%S.png", currentTime);
    stbi_write_png(exportFile, 288, 416, 4, flipped, width*4);
    free(troll);
    free(flipped);
    glBindFramebuffer(GL_FRAMEBUFFER, screenFB.fbo);
}

void RGBtoHSL(float *h, float *s, float *l, float r, float g, float b) {
    float cmax = 0.0;
    float cmin = 2.0;
    float deltac;
    if (cmax < r) cmax = r;
    if (cmax < g) cmax = g;
    if (cmax < b) cmax = b;
    if (cmin > r) cmin = r;
    if (cmin > g) cmin = g;
    if (cmin > b) cmin = b;
    deltac = cmax - cmin;

    *l = (cmax + cmin) / 2.0;
    if (deltac == 0) {
        *h = 0;
        *s = 0;
    } else if (cmax == r) {
        *h = (moduf((g - b)/deltac, 6.0))/6.0;
    } else if (cmax == g) {
        *h = (((b - r)/deltac) + 2.0)/6.0;
    } else if (cmax == b) {
        *h = (((r - g)/deltac) + 4.0)/6.0;
    }
    if (deltac != 0) {
        *s = deltac / (1 - fabs(2.0 * *l - 1));
    }
}

void HSLtoRGB(float *r, float *g, float *b, float h, float s, float l) {
    h *= 6.0;
    float c = (1 - fabs(2.0 * l - 1)) * s;
    float x = c * (1.0 - fabs(fmodf(h, 2.0) - 1.0));
    float m = l - c/2.0;
    
    if (h < 1) {
        *r = c;
        *g = x;
        *b = 0.0;
    } else if (1 <= h && h < 2) {
        *r = x;
        *g = c;
        *b = 0.0;
    } else if (2 <= h && h < 3) {
        *r = 0.0;
        *g = c;
        *b = x;
    } else if (3 <= h && h < 4) {
        *r = 0.0;
        *g = x;
        *b = c;
    } else if (4 <= h && h < 5) {
        *r = x;
        *g = 0.0;
        *b = c;
    } else {
        *r = c;
        *g = 0.0;
        *b = x;
    }
    *r += m;
    *g += m;
    *b += m;
}

int calcOdds(int *choices, int *odds, int length) {
    int oddsSum = 0;
    for (int i = 0; i < length; i++) {
        oddsSum += odds[i];
    }
    int randPick = SDL_rand(oddsSum);
    int newSum = 0;
    for (int i = 0; i < length; i++) {
        if (randPick >= newSum && randPick < newSum + odds[i]) {
            return choices[i];
        }
        newSum += odds[i];
    }
    return 0;
}

void setColorFromHex(layertex *layer, int color) {
    layer->r = (float)((color & 0xff0000) >> 16) / 255;
    layer->g = (float)((color & 0x00ff00) >> 8) / 255;
    layer->b = (float)(color & 0x0000ff) / 255;
}

void copyColors(layertex *dest, layertex *src) {
    dest->r = src->r;
    dest->g = src->g;
    dest->b = src->b;
}

void fixEyeType(layertex *layer, int *choices, int *odds, int length, int regEyeColor, int blEyeColor) {
    int layerAsset = layer->asset;
    int eyesType = calcOdds(choices, odds, length);
    int eyeVal = (layerAsset - 1) % 3;

    if (eyesType == 0) { // regular
        if (eyeVal == 1) {
            loadCreatorTextureNoColor(layer, layerAsset - 1);
        } else if (eyeVal == 2) {
            loadCreatorTextureNoColor(layer, layerAsset - 2);
        }
        setColorFromHex(layer, regEyeColor);
    } else if (eyesType == 1) { // blank
        if (eyeVal == 0) {
            loadCreatorTextureNoColor(layer, layerAsset + 1);
        } else if (eyeVal == 2) {
            loadCreatorTextureNoColor(layer, layerAsset - 1);
        }
        setColorFromHex(layer, blEyeColor);
    } else { // weird
        if (eyeVal == 0) {
            loadCreatorTextureNoColor(layer, layerAsset + 2);
        } else if (eyeVal == 1) {
            loadCreatorTextureNoColor(layer, layerAsset + 1);
        }
        setColorFromHex(layer, regEyeColor);
    } 
}

void leftEyeCheck(int regEyeColor, int blEyeColor) {
    int eyeVal1 = eyeR.asset % 3;
    int eyeVal2 = eyeL.asset % 3;

    if (eyeVal1 == 1) {
        if (eyeVal2 == 1) {
            copyColors(&eyeL, &eyeR);
        } else if (eyeVal2 == 2) {
            setColorFromHex(&eyeL, blEyeColor);
            setColorFromHex(&eyeR, 0x222222);
        } else {
            if (caste == 7) {
                copyColors(&eyeL, &eyeR);
            } else {
                int lEyesChoices[2] = {0, 1};
                int lEyesOdds[2] = {50, 50};
                fixEyeType(&eyeL, lEyesChoices, lEyesOdds, 2, regEyeColor, blEyeColor);
                leftEyeCheck(regEyeColor, blEyeColor);
            }
        }
    } else if (eyeVal1 == 2) {
        if (eyeVal2 == 1) {
            setColorFromHex(&eyeL, 0x222222);
        } else if (eyeVal2 == 2) {
            copyColors(&eyeL, &eyeR);
        } else {
            if (caste == 7) {
                setColorFromHex(&eyeL, 0x222222);
            } else {
                int lEyesChoices[2] = {0, 1};
                int lEyesOdds[2] = {50, 50};
                fixEyeType(&eyeL, lEyesChoices, lEyesOdds, 2, regEyeColor, blEyeColor);
                leftEyeCheck(regEyeColor, blEyeColor);
            }
        }
    }
}

void randomizeTrollPerfect(int *colorChoices, int *colorChoices2, int randomColor, int regEyeColor, int blEyeColor) {
    int hornsChoices[2] = {hornL.asset, hornR.asset};
    int hornsOdds[2] = {90, 10};
    loadCreatorTextureNoColor(&hornR, calcOdds(hornsChoices, hornsOdds, 2));

    // back, hats, face, cloth, mouth
    int odds1[5][5] = { {2, 60, 2, 35, 1},
                        {5, 80, 5, 5, 5},
                        {15, 30, 0, 5, 50},
                        {20, 30, 5, 40, 5},
                        {25, 25, 45, 5, 0}
                    };

    int coloredClothOdds[5] = {40, 35, 20, 1, 4};
    int blackClothOdds[5] = {0, 45, 30, 5, 20};
    int whiteClothOdds[5] = {35, 40, 20, 5, 0};

    int regularHairOdds[5] = {95, 1, 0, 1, 3};
    int coloredHairOdds[5] = {2, 75, 3, 15, 5};

    layertex *parts1[5] = {&acc_back, &acc_hats, &acc_face, &acc_cloth, &mouth};
    for (int i = 0; i < 5; i++) {
        int color = calcOdds(colorChoices, odds1[i], 5);
        setColorFromHex(parts1[i], color);
    }

    // hair
    int randPick1 = calcOdds(colorChoices, regularHairOdds, 5);
    int randPick2 = calcOdds(colorChoices, coloredHairOdds, 5);
    if (hair.asset <= 145) {
        if (bangs.asset <= 88) {
            setColorFromHex(&bangs, randPick1);
            copyColors(&hair, &bangs);
        } else {
            setColorFromHex(&bangs, randPick2);
            hair.r = hair.g = hair.b = 34.0/255.0;
        }
    } else {
        if (bangs.asset <= 88) {
            bangs.r = bangs.g = bangs.b = 34.0/255.0;
            setColorFromHex(&hair, randPick2);
        } else {
            setColorFromHex(&bangs, randPick1);
            copyColors(&hair, &bangs);
        }
    }

    // upper and sigil
    if (upper.asset <= 86) {
        randPick1 = calcOdds(colorChoices, coloredClothOdds, 5);
    } else if (upper.asset <= 206) {
        randPick1 = calcOdds(colorChoices, blackClothOdds, 5);
    } else {
        randPick1 = calcOdds(colorChoices, whiteClothOdds, 5);
    }
    setColorFromHex(&upper, randPick1);

    int sigilOdds[4] = {0};
    if (randPick1 == 0x222222) {
        sigilOdds[0] = 5; sigilOdds[1] = 75; sigilOdds[2] = 0; sigilOdds[3] = 10;
    } else if (randPick1 == 0xffffff) {
        sigilOdds[0] = 50; sigilOdds[1] = 40; sigilOdds[2] = 0; sigilOdds[3] = 5;
    } else if (randPick1 == randomColor) {
        sigilOdds[0] = 60; sigilOdds[1] = 30; sigilOdds[2] = 0; sigilOdds[3] = 10;
    } else {
        sigilOdds[0] = 65; sigilOdds[1] = 5; sigilOdds[2] = 0; sigilOdds[3] = 30;
    }
    int sigilColor = calcOdds(colorChoices2, sigilOdds, 4);
    setColorFromHex(&sigil, sigilColor);

    // bottom and shoes
    if (bottom.asset <= 72) {
        randPick1 = calcOdds(colorChoices, coloredClothOdds, 5);
    } else if (bottom.asset <= 93) {
        randPick1 = calcOdds(colorChoices, blackClothOdds, 5);
    } else {
        randPick1 = calcOdds(colorChoices, whiteClothOdds, 5);
    }
    setColorFromHex(&bottom, randPick1);

    if (shoeL.asset <= 21) {
        randPick1 = calcOdds(colorChoices, coloredClothOdds, 5);
    } else if (shoeL.asset <= 51) {
        randPick1 = calcOdds(colorChoices, blackClothOdds, 5);
    } else {
        randPick1 = calcOdds(colorChoices, whiteClothOdds, 5);
    }
    setColorFromHex(&shoeL, randPick1);
    copyColors(&shoeR, &shoeL);

    // eyes
    loadCreatorTextureNoColor(&eyeR, eyeL.asset);
    int rEyesChoices[2] = {0, 1};
    int rEyesOdds[2] = {85, 15};
    fixEyeType(&eyeR, rEyesChoices, rEyesOdds, 2, regEyeColor, blEyeColor);

    int lEyesChoices[3] = {0, 1, 2};
    int lEyesOdds[3] = {85, 15, 0};
    if (caste == 7) {
        lEyesOdds[0] = 45; lEyesOdds[1] = 10; lEyesOdds[2] = 45;
    }
    fixEyeType(&eyeL, lEyesChoices, lEyesOdds, 3, regEyeColor, blEyeColor);

    leftEyeCheck(regEyeColor, blEyeColor);

    if (caste > 9) {
        loadCreatorTextureNoColor(&body, 2);
    } else {
        loadCreatorTextureNoColor(&body, 1);
    }

    if (caste != 9) {
        loadCreatorTextureNoColor(&facepaint, 0);
    }

}

void randomizeTrollImperfect(void) {

}

void randomizeTroll(bool sym) {
    loadCreatorTextureNoColor(&bg, 1);
    loadCreatorTextureNoColor(&acc_back, SDL_rand(AMT_ACC_BACK+1));
    loadCreatorTextureNoColor(&hair, SDL_rand(AMT_HAIR+1));
    loadCreatorTextureNoColor(&body, SDL_rand(AMT_BODY)+1);
    loadCreatorTextureNoColor(&markings, SDL_rand(AMT_MARKINGS+1));
    loadCreatorTextureNoColor(&facepaint, SDL_rand(AMT_FACEPAINT+1));
    loadCreatorTextureNoColor(&mouth, SDL_rand(AMT_MOUTH+1));
    loadCreatorTextureNoColor(&eyebrows, SDL_rand(AMT_EYEBROWS+1));
    loadCreatorTextureNoColor(&eyeL, SDL_rand(AMT_EYE+1));
    loadCreatorTextureNoColor(&eyeR, SDL_rand(AMT_EYE+1));
    loadCreatorTextureNoColor(&shoeL, SDL_rand(AMT_SHOE+1));
    loadCreatorTextureNoColor(&shoeR, shoeL.asset);
    loadCreatorTextureNoColor(&bottom, SDL_rand(AMT_BOTTOM+1));
    loadCreatorTextureNoColor(&acc_cloth, SDL_rand(AMT_ACC_CLOTH+1));
    loadCreatorTextureNoColor(&upper, SDL_rand(AMT_UPPER+1));
    loadCreatorTextureNoColor(&sigil, SDL_rand(AMT_SIGIL+1));
    loadCreatorTextureNoColor(&acc_face, SDL_rand(AMT_ACC_FACE+1));
    loadCreatorTextureNoColor(&hornL, SDL_rand(AMT_HORN+1));
    loadCreatorTextureNoColor(&hornR, SDL_rand(AMT_HORN+1));
    loadCreatorTextureNoColor(&bangs, SDL_rand(AMT_BANGS+1));
    loadCreatorTextureNoColor(&acc_hats, SDL_rand(AMT_ACC_HATS+1));

    body.r = 196.0/255.0;
    body.g = body.b = body.r;

    caste = SDL_rand(12);
    int randomColor = ((SDL_rand(150) + 50) << 16) + (SDL_rand(200) << 8) + SDL_rand(200);

    int colorChoices[5] = {0x222222, hemo[caste], 0x6c6c6c, randomColor, 0xffffff};
    int colorChoices2[5] = {0x000000, hemo[caste], randomColor, 0xffffff};
    int blEyesOdds[5] = {10, 30, 0, 10, 50};
    int regEyesOdds[4] = {70, 27, 3, 0};
    int blEyeColor = calcOdds(colorChoices, blEyesOdds, 5);
    int regEyeColor = calcOdds(colorChoices2, regEyesOdds, 4);
    
    // rare assets
    layertex *rareAssets[5] = {&acc_hats, &acc_face, &acc_cloth, &acc_back, &markings};
    int partsOdds[2] = {90, 10};
    for (int i = 0; i < 5; i++) {
        int partsChoices[2] = {0, rareAssets[i]->asset};
        loadCreatorTextureNoColor(rareAssets[i], calcOdds(partsChoices, partsOdds, 2));
    }

    if (sym) randomizeTrollPerfect(colorChoices, colorChoices2, randomColor, regEyeColor, blEyeColor);
    else randomizeTrollImperfect();
}