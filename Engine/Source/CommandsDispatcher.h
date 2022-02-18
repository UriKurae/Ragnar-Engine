#pragma once

#include "Command.h"

#include <stack>

class CommandDispatcher
{
public:
	CommandDispatcher() {}

	static void Execute(Command* c)
	{
		c->Execute();
		commands.push(c);
	}

	static void Undo()
	{
		if (!commands.empty())
		{
			commands.top()->GenerateRedo();
			undoneCommands.push(commands.top());
			commands.top()->Undo();
			commands.pop();
		}
	}

	static void Redo()
	{
		if (!undoneCommands.empty())
		{
			undoneCommands.top()->Redo();
			commands.push(undoneCommands.top());
			undoneCommands.pop();
		}
	}

	static void Shutdown()
	{
		while(!commands.empty())
		{
			Command* c = commands.top();
			commands.pop();
			delete c;
			c = nullptr;
		}

		while (!undoneCommands.empty())
		{
			if (Command* c = undoneCommands.top())
			{
				undoneCommands.pop();
				delete c;
				c = nullptr;
			}
		}
	}

private:

	static std::stack<Command*> commands;
	static std::stack<Command*> undoneCommands;
};

std::stack<Command*> CommandDispatcher::commands = {};
std::stack<Command*> CommandDispatcher::undoneCommands = {};