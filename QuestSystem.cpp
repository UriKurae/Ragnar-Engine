#include "Application.h"
#include "Globals.h"
#include "QuestSystem.h"

QuestSystem::QuestSystem()
{
	
}

QuestSystem::~QuestSystem()
{
	for (auto& q : questList)
	{
		delete q;
		q = nullptr;
	}

	questList.clear();
}

void QuestSystem::CreateQuest(uint _id, std::string _name, std::string _description, QuestState _state, QuestType _type)
{
	Quest* newQuest = nullptr;
	newQuest = new Quest(_id, _name, _description, _state, _type);
	AddQuest(newQuest);
}

void QuestSystem::AddQuest(Quest* questToAdd)
{
	questList.push_back(questToAdd);
	activeQuestsList.push_back(questToAdd);
}

void QuestSystem::CompleteQuest(Quest* questToComplete)
{
	completedQuestsList.push_back(questToComplete);
	questToComplete->ChangeState(QuestState::COMPLETED);
	activeQuestsList.remove(questToComplete);
}

void QuestSystem::RemoveQuest(Quest *questToDelete)
{
	questList.remove(questToDelete);
	delete questToDelete;
}

void QuestSystem::ChangeQuestState(Quest* questToChange, QuestState newState)
{
	questToChange->ChangeState(newState);
}
