#pragma once

enum class CommandType
{
	NONE = 0,
	MOUSE_MOVE,
};

#define COMMAND_TYPE(t)	static CommandType GetStaticType() { return CommandType::##t; }\
						virtual CommandType GetType() const override { return GetStaticType(); }

class Command
{
public:
	virtual ~Command() {};
	virtual void Execute() {};
	virtual void Undo() {};
	virtual void Redo() {};
	virtual void GenerateRedo() {};

	virtual CommandType GetType() const { return type; }

protected:
	CommandType type;
};
