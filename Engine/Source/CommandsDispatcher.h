#pragma once

#include "Command.h"

#include <stack>



class CommandDispatcher
{
public:
	CommandDispatcher() {}
	//CommandDispatcher() {};

	template <typename T>
	void Execute(Command* c)
	{
		//if (c.GetType() == T::GetStaticType())
		{
			c->Execute();
			commands.push(c);
		}
	}

	//template <typename T>
	void Undo()
	{
		if (!commands.empty())
		{
			commands.top()->Undo();
			undoneCommands.push(commands.top());
			commands.pop();
		}
	}


private:

	static std::stack<Command*> commands;
	static std::stack<Command*> undoneCommands;
	static std::stack<Command*> redoneCommands;
};

std::stack<Command*> CommandDispatcher::commands = {};
std::stack<Command*> CommandDispatcher::undoneCommands = {};
std::stack<Command*> CommandDispatcher::redoneCommands = {};