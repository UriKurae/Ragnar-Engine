#pragma once

class Application;

class Module
{
private :
	bool enabled;

public:
	Application* app;

	Module(Application* parent, bool startEnabled = true) : app(parent), enabled(startEnabled)
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

	virtual bool PreUpdate(float dt)
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool PostUpdate()
	{
		return true;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}
};