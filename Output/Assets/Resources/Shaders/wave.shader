#type vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

uniform vec3 ambientColor;
uniform int time;
uniform float amplitude;
uniform float frequency;
uniform float speed;
uniform float textureAlpha;
uniform float foamSpeed;
uniform vec2 foamDir;
uniform float noiseAmount;

out vec3 vPosition;
out vec3 vNormal;
out vec3 vAmbientColor;
out vec2 vTexCoords;
out float vAmplitude;
out float vTextureAlpha;

float pi = 3.14159265359;

float hash(float n)
{
	return fract(cos(n) * 41415.92653);
}

float noise(in vec3 x)
{
	vec3 p = floor(x);
	vec3 f = smoothstep(0.0, 1.0, fract(x));
	float n = p.x + p.y * 57.0 + 113.0 * p.z;

	return mix(mix(mix(hash(n + 0.0), hash(n + 1.0), f.x),
		mix(hash(n + 57.0), hash(n + 58.0), f.x), f.y),
		mix(mix(hash(n + 113.0), hash(n + 114.0), f.x),
			mix(hash(n + 170.0), hash(n + 171.0), f.x), f.y), f.z);
}


float random(vec2 p)
{
	vec2 K1 = vec2(23.14069263277926, 2.665144142690225);

	//return fract( cos( mod( 12345678., 256. * dot(p,K1) ) ) ); // ver1
	return fract(cos(dot(p, K1)) * 12345.6789); // ver3
}

float random2(vec2 co)
{
	return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
	vec3 pos = position;
	pos.y = amplitude * 2 * pi * sin((time * 0.1) * speed * 0.1 - pos.x * frequency);

	//pos.y += noise(pos)
	pos.y += random(tan(pos.xz)) * noiseAmount;

	// Line below makes waves not go deep equaly
	pos.zy += sin(pos.zx);
	//pos.zy += mix(sin(pos.zx), cos(pos.xx), 1.50);


	gl_Position = projection * view * model * vec4(pos, 1.0);

	vAmbientColor = pos.y * ambientColor;

	vec2 tcDir = normalize(foamDir);

	vec2 tc = texCoords;
	tc.x -= foamSpeed * time * 0.001; //* tcDir.x;
	tc.y -= foamSpeed * time * 0.001; //* tcDir.y;
	vTexCoords = tc;

	vPosition = vec3(model * vec4(position, 1));
	//vNormal = normalMatrix * normal;
	vNormal = normalize((model * vec4(normal, 0.0)).xyz);
	vAmplitude = amplitude;
	vTextureAlpha = textureAlpha;
	//vAmbientColor = ambientColor;	

}


#type fragment
#version 430 core

in vec3 vPosition;
in vec3 vNormal;
in vec3 vAmbientColor;
in vec2 vTexCoords;
in float vAmplitude;
in float vTextureAlpha;

out vec4 fragColor;

uniform sampler2D tex;

void main()
{
	vec3 norm = normalize(vNormal);
	vec3 lightPos = vec3(20, 10, 5);
	vec3 lightDir = normalize(lightPos - vPosition);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 lightColor = vec3(1, 1, 0.1);
	vec3 diffuse = diff * lightColor;

	vec3 ambient = vec3(0.8, 0.2, 0.2);
	vec3 result = (ambient + diffuse) * vAmbientColor;

	vec3 col = mix(vec3(0.0, 0.0, 0.4), -vAmbientColor, -1.0); // -1.0 its for white intesnity

	fragColor = texture(tex, vTexCoords) * vec4(col, vTextureAlpha);
}
