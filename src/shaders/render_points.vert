#version 130

uniform mat4 mvp;

in vec3 position;

in vec4 color;

out vec4 color2;

void
main()
{
  color2 = color;

  gl_PointSize = 4;

  gl_Position = mvp * vec4(position, 1.0);
}
