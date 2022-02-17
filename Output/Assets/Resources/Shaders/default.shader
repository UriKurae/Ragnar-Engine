#type vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform float textureAlpha;
uniform vec3 ambientColor;
uniform vec3 camPos;

out vec3 vPosition;
out vec3 vAmbientColor;
out vec2 vTexCoords;
out vec3 vCamPos;
out vec3 vNormal;
out float vTextureAlpha;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1);

	vTexCoords = texCoords;
	vPosition = vec3(model * vec4(position, 1));
	//vNormal = normalMatrix * normal;
	vNormal = normalize((model * vec4(normal, 0.0)).xyz);
	vAmbientColor = ambientColor;
	vTextureAlpha = textureAlpha;

	vCamPos = camPos;
}


#type fragment
#version 430 core

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoords;
in vec3 camPos;

in vec3 vAmbientColor;
in float vTextureAlpha;

out vec4 fragColor;

uniform sampler2D tex;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	bool gammaCorrection;
	float gammaCorrectionAmount;
};

uniform Material material;

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dirLight;

struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// 0 -> constant, 1 -> linear (lin), 2 -> quadratic
	vec3 properties;

	float intensity;
	float constant;
	float lin;
	float quadratic;
};
#define MAX_POINT_LIGHTS 4
uniform PointLight pointLights[MAX_POINT_LIGHTS];

//struct SpotLight
//{
//	vec3 position;
//	vec3 direction;
//	float cutOff;
//	float outerCutOff;
//	float intensity;
//
//	vec3 ambient;
//	vec3 diffuse;
//	vec3 specular;
//
//};
//#define MAX_SPOT_LIGHTS 4
//uniform SpotLight spotLights[MAX_SPOT_LIGHTS];


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	
	// Diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	
	// Specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
	vec3 ambient = light.ambient * material.diffuse * vAmbientColor;
	vec3 diffuse = light.diffuse * diff * material.diffuse;
	vec3 specular = light.specular * spec * material.specular;

	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	// Diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);

	// Specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	// Attenuation
	float distance = length(fragPos - light.position);
	float attenuation = 1.0 / (1 + light.constant + light.lin * distance + light.quadratic * (distance * distance));
	//float attenuation = 1.0 / (light.constant + light.lin * distance + light.quadratic * (distance * distance));
	//float attenuation = 1.0 / (light.properties[0] + light.properties[1] * distance + light.properties[2] * (distance * distance));
	
	attenuation *= light.intensity;

	vec3 ambient = light.ambient * material.diffuse * vAmbientColor;
	vec3 diffuse = light.diffuse * diff * material.diffuse;
	vec3 specular = light.specular * spec * material.specular;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

void main()
{
	vec3 norm = normalize(vNormal);
	vec3 viewDir = normalize(camPos - vPosition);

	vec3 result = CalcDirLight(dirLight, norm, viewDir);

	for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
		result += CalcPointLight(pointLights[i], norm, vPosition, viewDir);

	vec3 finalColor = result;
	if (material.gammaCorrection)
	{
		finalColor = pow(result, vec3(1.0 / material.gammaCorrectionAmount));
	}

	fragColor = texture(tex , vTexCoords) * vTextureAlpha * vec4(finalColor, 1);
}
