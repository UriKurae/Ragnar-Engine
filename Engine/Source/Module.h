#pragma once

class JsonParsing;

class Module
{
public:

	Module(bool startEnabled = true) : enabled(startEnabled), name("")
	{}

	virtual ~Module()
	{}

	virtual bool Init(JsonParsing& node)
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

	virtual bool LoadConfig(JsonParsing& node)
	{
		return true;
	}

	virtual bool SaveConfig(JsonParsing& node) const
	{
		return true;
	}

public:
	const char* name;

private:
	bool enabled;
};