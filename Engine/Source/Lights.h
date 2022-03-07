#pragma once
#include "Math/float3.h"

enum class LightType
{
    NONE = 0,
    DIRECTIONAL,
    POINT,
    SPOT
};

class Light
{
public:
    LightType type = LightType::NONE;

    //float3 ambient = { 1,0,0 };
    //float3 diffuse = { 1,1,1 };
    //float3 specular = { 0,0,1 };

    float3 ambient = { 0.25f, 0.25f, 0.25f };
    float3 diffuse = { 0.75f, 0.75f, 0.75f };
    float3 specular = { 1,1,1 };
};

class DirectionalLight : public Light
{
public:
    DirectionalLight()
    {
        dir = { 1,0,0 };
        ambient = { 0.25f, 0.25f, 0.25f };
        diffuse = { 0.75f, 0.75f, 0.75f };
        specular = { 1,1,1 };

        type = LightType::DIRECTIONAL;

        Light();
    }

public:
    float3 dir;
    //loat3 ambient;
    //loat3 diffuse;
    //loat3 specular;
};

class PointLight : public Light
{
public:
    PointLight()
    {
        position = { 0,0,0 };
        type = LightType::POINT;

        Light();
    }

public:
    float3 position = { 0,0,0 };

    float intensity = 10.0f;
    float constant = 1.0f;
    // Linear
    float lin = 0.09f;
    float quadratic = 0.032f;
};

class SpotLight : public Light
{
public:
    SpotLight()
    {
        position = { 0,0,0 };
        direction = { 0,0,-1 };
        cutOff = math::DegToRad(12.5f);
        outerCutOff = math::DegToRad(15.0f);
        intensity = 1.0f;

        type = LightType::SPOT;

        Light();
    }

public:
    float3  position;
    float3  direction;
    // Must be in radians since it's an angle!!
    float cutOff;
    float outerCutOff;
    float intensity;
};