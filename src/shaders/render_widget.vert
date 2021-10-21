#version 130

uniform mat4 mvp;

uniform float width;

uniform float height;

in vec2 position;

out vec2 texCoords;

void
main()
{
  texCoords = position;

  gl_Position = mvp * vec4(position.x * width, height - (position.y * height), 0, 1.0);
}
