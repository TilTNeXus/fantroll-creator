#version 330
precision mediump float;

in vec3 fragPos;
in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D linesTex;
uniform sampler2D tintTex;
uniform vec3 color;
uniform bool flip;

void main() {
    vec2 flipTexCoord = vec2(abs(float(flip) - texCoord.x), texCoord.y);
    vec4 linesColor = texture(linesTex, flipTexCoord);
    vec4 tintColor = vec4(color, texture(tintTex, flipTexCoord).a);
    float addedAlpha = linesColor.a + tintColor.a*(1.0 - linesColor.a);
    vec3 addedColor = (linesColor.rgb*linesColor.a + tintColor.rgb*tintColor.a*(1.0 - linesColor.a))/addedAlpha;
    fragColor = vec4(addedColor, addedAlpha);
}