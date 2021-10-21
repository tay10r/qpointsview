#version 130

uniform mat4 mvp;

uniform float pointRadius;

in vec3 position;

in vec4 color;

out vec4 color2;

void
main()
{
  color2 = color;

  gl_Position = mvp * vec4(position, 1.0);

  gl_PointSize = pointRadius / gl_Position.w;
}
