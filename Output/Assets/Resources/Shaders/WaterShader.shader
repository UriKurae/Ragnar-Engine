
#ifdef __Vertex_Shader__

#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

#define PI 3.1416
#define g 9.81

out vec4 ourColor;
out vec2 TexCoord;
out vec3 fNormal;
out vec3 fPos;

uniform vec4 inColor;

uniform float time;

uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;

uniform float wavelenghtA = 64;
uniform float steepnessA = 0.25;
uniform vec2 directionA = vec2(1, 1);

uniform float wavelenghtB = 32;
uniform float steepnessB = 0.25;
uniform vec2 directionB = vec2(1, 0.6);

uniform float wavelenghtC = 16;
uniform float steepnessC = 0.25;
uniform vec2 directionC = vec2(1, 1.3);

vec3 Wave(float wavelenght, float steepness, vec2 direction, vec3 pos, inout vec3 tan, inout vec3 binormal)
{


    float k = (2 * PI) / wavelenght;
    float a = steepness / k;
    float c = sqrt(g / k);
    vec2 d = normalize(direction);
    float f = (dot(d, pos.xy) - c * time) * k;

    tan += vec3(-(d.x * d.x) * (a * sin(f)), -(d.x * d.y) * (a * sin(f)), d.x * (a * cos(f)));

    binormal += vec3(-(d.x * d.y) * (a * sin(f)), -(d.y * d.y) * (a * sin(f)), d.y * (a * cos(f)));

    return vec3((cos(f) * a) * d.x, (cos(f) * a) * d.y, sin(f) * a);
}


void main()
{
    vec3 pos;
    pos = position;

    vec3 binormal = vec3(0, 1, 0);
    vec3 tan = vec3(1, 0, 0);

    pos += Wave(wavelenghtA, steepnessA, directionA, position, tan, binormal);
    pos += Wave(wavelenghtB, steepnessB, directionB, position, tan, binormal);
    pos += Wave(wavelenghtC, steepnessC, directionC, position, tan, binormal);

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(pos, 1.0f);

    fNormal = normalize(cross(binormal, tan));
    ourColor = inColor;
    TexCoord = texCoord;
    fPos = pos;
}

#endif

//--------------------

#ifdef __Fragment_Shader__

#version 430 core

in vec4 ourColor;
in vec2 TexCoord;
in vec3 fNormal;
in vec3 fPos;
out vec4 color;

uniform float Ka = 0.5;
uniform float Kd = 4.5;
uniform float Ks = 0.0;
uniform float shininess = 100;


uniform sampler2D ourTexture;

vec2 Direction(vec3 lightDir, float lightInt, float Ka, float Kd, float Ks, float shininess)
{
    vec3 s = normalize(lightDir);
    vec3 v = normalize(-fPos);
    vec3 n = normalize(fNormal);
    vec3 h = normalize(v + s);
    float diffuse = Ka + Kd * lightInt * max(0.0, dot(n, s));
    float spec = Ks * pow(max(0.0, dot(n, h)), shininess);
    return vec2(diffuse, spec);
}

void main()
{

    vec3 lcolor;
    vec3 dark_color = vec3(0, 0.2, 0.3);
    vec3 blue = vec3(0, 0.5, 0.7);
    vec3 foam_color = vec3(0.9, 0.95, 1);
    float h = 0.9;
    float range = distance(fPos.z / 2, -5) / 5;

    if (fPos.z > texture(ourTexture, TexCoord).r * 2 + 1)
    {
        float mixvalue = distance(fPos.z / 4, 0);
        lcolor = mix(blue, foam_color, min(mixvalue, 1));
    }
    else
    {
        float mixvalue = distance(fPos.z / 2, -3) / 3;
        lcolor = blue;
        vec4 tex_color = texture2D(ourTexture, TexCoord);
        lcolor += tex_color.rgb * (0, 0.7, 0.9) * 0.4;

        lcolor = mix(dark_color, lcolor, min(mixvalue, 1));
    }

    vec2 inten = Direction(vec3(1, 1, 0), 0.3, Ka, Kd, Ks, shininess);

    vec4 tex_color = texture2D(ourTexture, TexCoord);

    color = vec4(lcolor * inten.x + vec3(1.0) * inten.y, 1.0);
}

#endif








