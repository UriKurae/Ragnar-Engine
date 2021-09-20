#pragma once

class Application;

class Module
{
private :
	bool enabled;

public:
	Application* app;

	Module(Application* parent, bool start_enabled = true) : app(parent)
	{}

	virtual ~Module()
	{}

	virtual bool Init() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual UpdateStatus PreUpdate(float dt)
	{
		return UpdateStatus::UPDATE_CONTINUE;
	}

	virtual UpdateStatus Update(float dt)
	{
		return UpdateStatus::UPDATE_CONTINUE;
	}

	virtual UpdateStatus PostUpdate()
	{
		return UpdateStatus::UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}
};