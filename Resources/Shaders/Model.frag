#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D diffuse1;
uniform sampler2D specular1;
uniform sampler2D normalmap1;

void main()
{    
    color = texture(normalmap1, TexCoords);
}