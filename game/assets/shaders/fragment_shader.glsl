#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

void main()
{
    // Example: Checkerboard pattern
    float checker = mod(floor(TexCoord.x * 10.0) + floor(TexCoord.y * 10.0), 2.0);
    FragColor = vec4(vec3(1.0,1.0,1.0), 1.0);
}