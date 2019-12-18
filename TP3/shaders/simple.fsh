varying vec4 uv;
uniform sampler2D texture;

void main() {
      gl_FragColor = texture2D(texture, uv.st);
}
