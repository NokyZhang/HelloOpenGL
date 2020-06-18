
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

out vec3 vertexColor;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
   gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
   vertexColor = vec3(aColor.x, aColor.y, aColor.z);
   TexCoord = aTexCoord;
   FragPos = vec3(model * vec4(aPos.x, aPos.y, aPos.z, 1.0));
   Normal = mat3(transpose(inverse(model))) * aNormal;
}