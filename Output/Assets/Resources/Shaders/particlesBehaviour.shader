#type vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec4 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 vTexCoords;
out vec4 vColor;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	vColor = color;
	vTexCoords = texCoords;
}

#type fragment
#version 430 core

layout(location = 0) out vec4 fragColor;

uniform sampler2D tex;

in vec2 vTexCoords;
in vec4 vColor;

void main()
{
	fragColor = texture(tex, vTexCoords) * vColor;
}
