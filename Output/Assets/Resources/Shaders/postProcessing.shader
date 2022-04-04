#type vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;

out vec2 vTexCoords;
out vec3 vPosition;

void main()
{
	gl_Position = vec4(position, 1.0f);
	vTexCoords = texCoords;
	vPosition = position;
}

#type fragment
#version 430 core

in vec2 vTexCoords;
in vec3 vPosition;

out vec4 fragColor;
uniform sampler2D colorTexture;
uniform sampler2D normalTexture;
uniform sampler2D depthTexture;

void main()
{
	vec2 texCoord = vec2(0);
	vec2 texSize = textureSize(colorTexture, 0).xy;

	float minDistance = 0.5;
	float maxDistance = 2.0;

	vec4 realPixelDepth = texture(depthTexture, vTexCoords);
	vec4 realPixelNormal = texture(normalTexture, vTexCoords);

	vec4 result = texture(colorTexture, vTexCoords);
	if (vTexCoords.y > 0.002 && vTexCoords.y < 0.998 && vTexCoords.x > 0.002 && vTexCoords.x < 0.998)
	{
		float maxDepth = 0;
		float maxNormal = 0;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				// Get the current surrounding pixel texCoords
				texCoord = (gl_FragCoord.xy + vec2(i, j)) / texSize;
				vec4 currentPixelDepth = texture(depthTexture, texCoord);
				vec4 currentPixelNormal = texture(normalTexture, texCoord);

				maxDepth = max(maxDepth, length(currentPixelDepth - realPixelDepth));
				maxNormal = max(maxNormal, length(currentPixelNormal - realPixelNormal));
			}
		}

		float normalThreshold = 0.2f;
		float depthThreshold = 0.01f;

		if (maxNormal > normalThreshold || maxDepth > depthThreshold)
			result = vec4(0, 0, 0, 1);

	}

	fragColor = result;
}
