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
	//friend class CommandDispatcher;

public:
	//Command(CommandType t) : type(t) {}
	virtual ~Command() {};
	virtual void Execute() {};
	virtual void Undo() {};

	virtual CommandType GetType() const { return type; }

protected:
	CommandType type;
};
