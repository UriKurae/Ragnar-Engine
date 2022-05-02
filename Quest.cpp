#include "Quest.h"

Quest::Quest(uint _id, std::string _name, std::string _description, QuestState _state, QuestType _type)
{
	id = _id;
	name = _name;
	description = _description;
	state = _state;
	type = _type;
}

Quest::~Quest()
{

}

void Quest::ChangeState(QuestState newState)
{
	state = newState;
}

uint Quest::GetQuestId()
{
	return id;
}

std::string Quest::GetQuestName()
{
	return name;
}

std::string Quest::GetQuestDescription()
{
	return description;
}

QuestState Quest::GetQuestState()
{
	return state;
}

QuestType Quest::GetQuestType()
{
	return type;
}
