#include "program.h"
#include "text.h"

void loadFont(const char *fontpath) {
    FT_Library ft;
    FT_Face face;
    FT_Init_FreeType(&ft);
    FT_New_Face(ft, fontpath, 0, &face);
    FT_Select_Charmap(face, FT_ENCODING_UNICODE);
    FT_Set_Pixel_Sizes(face, 0, 24);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (unsigned char c = 0; c < 128; c++) {
        FT_Load_Char(face, c, FT_LOAD_RENDER);
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        characters[c].textureID = texture;
        characters[c].sizeX = face->glyph->bitmap.width;
        characters[c].sizeY = face->glyph->bitmap.rows;
        characters[c].bearingX = face->glyph->bitmap_left;
        characters[c].bearingY = face->glyph->bitmap_top;
        characters[c].advance = face->glyph->advance.x;
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void drawText(unsigned int shader, char *text, float x, float y, float scale, float color[3]) {
    glUseProgram(shader);
    mat4f_init(model);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, projection);
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, model);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(textmodel.vao);

    for (char *ptr = text; *ptr; ptr++) {
        character ch = characters[(unsigned char)*ptr];
        float xpos = x + ch.bearingX * scale;
        float ypos = y - (ch.sizeY - ch.bearingY) * scale;
        float w = ch.sizeX * scale;
        float h = ch.sizeY * scale;

        float verts[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }  
        };

        glUniform3f(glGetUniformLocation(shader, "textColor"), color[0], color[1], color[2]);
        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        glBindBuffer(GL_ARRAY_BUFFER, textmodel.vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}