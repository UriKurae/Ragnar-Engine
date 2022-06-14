#pragma once
#include <stack>

class Command;

class CommandDispatcher
{
public:
	CommandDispatcher() {}
  
	static void Execute(Command* c);
	static void Undo();
	static void Redo();
	static void Shutdown();

private:

	static std::stack<Command*> commands;
	static std::stack<Command*> undoneCommands;
};

