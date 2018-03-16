#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;

uniform sampler2D diffuse1;
uniform sampler2D specular1;

void main()
{   
    vec3 norm = normalize(Normal);
    float diffuse = max( dot( norm, vec3( 1.0, 1.0, 1.0 ) ), 0.0 );
	FragColor = vec4((diffuse * texture(specular1, TexCoords)).xy,1.0,1.0);
    FragColor = vec4((diffuse * texture(diffuse1, TexCoords)).xyz, 1.0);
}
