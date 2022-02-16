#pragma once

#include "Command.h"

#include <stack>

class CommandDispatcher
{
public:
	CommandDispatcher() {}

	template <typename T>
	void Execute(Command* c)
	{
		c->Execute();
		commands.push(c);
	}

	void Undo()
	{
		if (!commands.empty())
		{
			commands.top()->GenerateRedo();
			undoneCommands.push(commands.top());
			commands.top()->Undo();
			commands.pop();
		}
	}

	void Redo()
	{
		if (!undoneCommands.empty())
		{
			undoneCommands.top()->Redo();
			commands.push(undoneCommands.top());
			undoneCommands.pop();
		}
	}

private:

	static std::stack<Command*> commands;
	static std::stack<Command*> undoneCommands;
	static std::stack<Command> redoneCommands;
};

std::stack<Command*> CommandDispatcher::commands = {};
std::stack<Command*> CommandDispatcher::undoneCommands = {};
std::stack<Command> CommandDispatcher::redoneCommands = {};