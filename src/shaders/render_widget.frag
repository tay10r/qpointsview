#version 130

in vec2 texCoords;

uniform sampler2D widgetTexture;

void
main()
{
  gl_FragColor = texture(widgetTexture, texCoords);
}
