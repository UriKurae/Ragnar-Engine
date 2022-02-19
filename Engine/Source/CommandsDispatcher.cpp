#include "CommandsDispatcher.h"

std::stack<Command*> CommandDispatcher::commands = {};
std::stack<Command*> CommandDispatcher::undoneCommands = {};

void CommandDispatcher::Execute(Command* c)
{
	c->Execute();
	commands.push(c);
}

void CommandDispatcher::Undo()
{
	if (!commands.empty())
	{
		commands.top()->GenerateRedo();
		undoneCommands.push(commands.top());
		commands.top()->Undo();
		commands.pop();
	}
}

void CommandDispatcher::Redo()
{
	if (!undoneCommands.empty())
	{
		undoneCommands.top()->Redo();
		commands.push(undoneCommands.top());
		undoneCommands.pop();
	}
}

void CommandDispatcher::Shutdown()
{
	while (!commands.empty())
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
