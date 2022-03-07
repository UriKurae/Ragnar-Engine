#pragma once
//
//#include "Resource.h"
//#include "Timer.h"
//
//#include <string>
//
//class Shader;
//class Timer;
//class ComponentCamera;
//
//class Material : public Resource
//{
//public:
//	Material(uint uid, std::string& assets, std::string& library);
//	virtual ~Material();
//
//	void Bind(const float4x4& transform, ComponentCamera* cam);
//	void Unbind();
//
//	inline void SetName(const std::string& newName) { name = newName; }
//	inline const std::string& GetName() const { return name; }
//
//	inline Shader* GetShader() { return shader; }
//	void SetShader(Shader* s);
//
//public:
//	float3 ambientColor;
//	float3 diffuseColor;
//	float3 specularColor;
//	float shininess;
//
//	float frequency;
//	float speed;
//	float amplitude;
//	float textureAlpha;
//	float foamSpeed;
//	//float2 foamDir;
//	float noiseAmount;
//
//	Timer timer;
//
//private:
//	std::string name;
//
//	Shader* shader;
//
//};