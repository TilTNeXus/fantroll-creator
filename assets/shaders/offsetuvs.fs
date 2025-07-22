#version 330
precision mediump float;

in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D tex;
uniform float uOffset;
uniform float vOffset;
uniform float scaleX;
uniform float scaleY;

void main() {
    vec2 offsetTexCoord = vec2(texCoord.x*scaleX + uOffset, texCoord.y*scaleY + vOffset);
    fragColor = texture(tex, offsetTexCoord);
}