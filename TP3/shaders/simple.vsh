attribute vec4 in_position;
attribute vec4 in_uv;
varying vec4 uv;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main() {
   uv = in_uv;
   gl_Position = projectionMatrix * viewMatrix * modelMatrix * in_position;
}
