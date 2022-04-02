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

uniform vec2 nearFar;

void main()
{
	vec2 texCoord = vec2(0);
	vec2 texSize = textureSize(colorTexture, 0).xy;

	float minDistance = 0.5;
	float maxDistance = 2.0;

	vec4 realPixelDepth = texture(depthTexture, vTexCoords);
	vec4 realPixelNormal = texture(normalTexture, vTexCoords);

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
			
			maxDepth = max(maxDepth, abs(currentPixelDepth.y - realPixelDepth.y));
			maxNormal = max(maxNormal, abs(currentPixelNormal.y - realPixelNormal.y));
		}
	}

	vec4 result = texture(colorTexture, vTexCoords);
	float threshold = 0.1f;
	
	if (maxNormal > threshold)
		result = vec4(0, 0, 0, 1);

	fragColor = result;
}















//uniform vec2 nearFar;
//
//float minSeparation = 1.0;
//float maxSeparation = 3.0;
//float minDistance = 0.5;
//float maxDistance = 2.0;
//int   size = 1;
//vec3 colorModifier = vec3(0.324, 0.063, 0.099);
//
//void main()
//{
//	float near = nearFar.x;
//	float far = nearFar.y;
//
//	vec2 texSize = textureSize(colorTexture, 0).xy;
//	vec2 fragCoord = gl_FragCoord.xy;
//	vec2 texCoord = fragCoord / texSize;
//
//	vec4 position = vec4(vPosition, 1);
//
//	float depth = clamp(1.0 - ((far - position.y) / (far - near)), 0.0, 1.0);
//
//    float separation = mix(maxSeparation, minSeparation, depth);
//
//	float mx = 0.0;
//
//	for (int i = -size; i <= size; ++i)
//	{
//		for (int j = -size; j <= size; ++j)
//		{
//			texCoord = (fragCoord + (vec2(i, j) * separation)) / texSize;
//
//			vec4 positionTemp = position;
//
//			mx = max(mx, abs(position.y - positionTemp.y));
//		}
//	}
//
//	float diff = smoothstep(minDistance, maxDistance, mx);
//	vec3 lineColor = texture(colorTexture, texCoord).rgb * colorModifier;
//
//	fragColor = vec4(lineColor, diff);
//}


