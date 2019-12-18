varying vec4 uv;
//varying vec4 color;
uniform sampler2D texture;
uniform vec4 color;


void main() {
      //gl_FragColor = texture2D(texture, uv.st);
      vec4 tex = texture2D(texture, uv.st);
      //gl_FragColor = tex + vec4(uv[0], uv[1], uv[2], uv[3])*uv[3];
      gl_FragColor = color;
      //gl_FragColor = texture2D(texture, uv.st) * vec4(0.5,0.5,0.5,0.7);
}
