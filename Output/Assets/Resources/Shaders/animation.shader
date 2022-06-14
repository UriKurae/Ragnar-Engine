#type vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;
layout(location = 3) in vec4 boneIds; 
layout(location = 4) in vec4 weights;
	
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
	
const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];
	
out vec2 TexCoords;
	
void main()
{
    vec4 totalPosition = vec4(0.0f);
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        int currentBoneId = int(boneIds[i]);

        if(currentBoneId == -1)
            continue;
        if(currentBoneId >= MAX_BONES)
        {
            totalPosition = vec4(position,1.0f);
            break;
        }
        vec4 localPosition = finalBonesMatrices[currentBoneId] * vec4(position,1.0f);
        totalPosition += localPosition * weights[i];
        vec3 localNormal = mat3(finalBonesMatrices[currentBoneId]) * normal;
    }
		
    mat4 viewModel = view * model;
    gl_Position =  projection * viewModel * totalPosition;
    TexCoords = texCoords;
}

#type fragment
#version 430 core

in vec2 TexCoords;

out vec4 fragColor;

//uniform sampler2D tex;
layout(location = 0) uniform sampler2D tex;

void main()
{
    fragColor = texture(tex, TexCoords);
}