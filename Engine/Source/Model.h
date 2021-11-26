#pragma once

#include "Resource.h"

class Model : public Resource
{
public:
	Model(uint uid, std::string& assets, std::string& library);

	~Model();

	void Load() override;

private:

};