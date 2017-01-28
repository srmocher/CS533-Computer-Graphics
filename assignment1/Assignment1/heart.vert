#version 410 core

layout( location = 0 ) in vec4 vPosition;
uniform vec4 inputColor;
out vec4 iColor
void
main()
{
	iColor = inputColor;
    gl_Position = vPosition;
}
