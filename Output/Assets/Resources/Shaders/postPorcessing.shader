#type vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;

out vec2 vTexCoords;

void main()
{
	gl_Position = vec4(position, 1.0f);
	vTexCoords = texCoords;
}

#type fragment
#version 430 core

out vec4 fragColor;
layout(location = 0) uniform sampler2D tex;

void main()
{
	// Process data with normals etc (double for and check surroundings)

	fragColor = texture(tex, vTexCoords);
}


