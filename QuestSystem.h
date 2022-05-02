#pragma once
#include "Quest.h"

class QuestSystem
{
public:
	QuestSystem();
	~QuestSystem();
private:
	std::list<Quest*> questList;
	std::list<Quest*> completedQuestsList;
	std::list<Quest*> activeQuestsList;
public:
	void CreateQuest(uint _id, std::string _name, std::string _description, QuestState _state, QuestType _type);
	void AddQuest(Quest* questToAdd);
	void CompleteQuest(Quest* questToComplete);
	void RemoveQuest(Quest *quest);
	void ChangeQuestState(Quest* questToChange, QuestState newState);
};