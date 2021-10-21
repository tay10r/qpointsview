#version 130

in vec4 color2;

uniform float pointRadius;

void
main()
{
  vec2 circleCoord = 2.0 * gl_PointCoord.st - 1.0;

  if (dot(circleCoord, circleCoord) > 1.0)
    discard;

  gl_FragColor = color2;
}
