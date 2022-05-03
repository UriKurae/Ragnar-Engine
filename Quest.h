#pragma once
#include "stdint.h"
#include <string>

typedef unsigned int uint;

enum class QuestState
{
	ACTIVE = 0,
	INACTIVE,
	COMPLETED,
};

enum class QuestType
{
	STORY_QUEST = 0,
	SECONDARY_QUEST,
};

class Quest {
public:
	// Constructors and destructor
	Quest(uint id, std::string name, std::string description, QuestState state, QuestType type);
	~Quest();
private:
	// Vars
	uint id;
	std::string name;
	std::string description;
	QuestState state;
	QuestType type;
public:
	void ChangeQuestState(QuestState newState);
	uint GetQuestId();
	std::string GetQuestName();
	std::string GetQuestDescription();
	QuestState GetQuestState();
	QuestType GetQuestType();
};