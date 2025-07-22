#version 330
precision mediump float;

in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D tex;
uniform float opacity;

void main() {
    fragColor = texture(tex, texCoord) * vec4(1.0, 1.0, 1.0, 1.0-opacity);
}