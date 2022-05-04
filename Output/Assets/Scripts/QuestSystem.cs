using System;
using System.Collections.Generic;
using RagnarEngine;

public enum QuestState
{
	ACTIVE = 0,
	INACTIVE,
	COMPLETED,
	UNKNOWN,
};

public enum QuestType
{
	MAIN = 0,
	SECONDARY,
	UNKNOWN,
};

public class Quest
{
	// Constructors and destructor
	public Quest(uint _id, string _name, string _description, QuestState _state, QuestType _type)
    {
		id = _id;
		name = _name;
		description = _description;
		state = _state;
		type = _type;
    }
	// Variables
	uint id;
	string name;
	string description;
	QuestState state;
	QuestType type;
	// Public Methods
	public void ChangeQuestState(QuestState newState) { state = newState; }
	public uint GetQuestId() { return id; }
	public string GetQuestName() { return name; }
	public string GetQuestDescription() { return description; }
	public QuestState GetQuestState() { return state; }
	public QuestType GetQuestType() { return type; }
};
public class QuestSystem : RagnarComponent
{
	// Lists
	public List<Quest> questList;
	public List<Quest> completedQuestList;
	public List<Quest> activeQuestList;

	public GameObject questTitle;
	public GameObject questDescription;
	public GameObject questId;
	public GameObject questType;
	public GameObject questState;

	public Vector3 position;
	// Methods
	public void CreateQuest(uint _id, string _name, string _description, QuestState _state, QuestType _type)
    {
		Quest newQuest;
		newQuest = new Quest(_id, _name, _description, _state, _type);
		AddQuest(newQuest);
	}

	public void AddQuest(Quest questToAdd)
    {
		// PETA AL AÑADIR LISTAS
		questList.Add(questToAdd);
		activeQuestList.Add(questToAdd);
	}
	public void CompleteQuest(Quest questToComplete)
    {
		completedQuestList.Add(questToComplete);
		questToComplete.ChangeQuestState(QuestState.COMPLETED);
		activeQuestList.Remove(questToComplete);
	}
	public void RemoveQuest(Quest questToDelete)
    {
		questList.Remove(questToDelete);
	}
	public void ChangeQuestState(Quest questToChange, QuestState newState)
    {
		questToChange.ChangeQuestState(newState);
    }

	public void Start()
	{
		// Initialize Lists
		questList = new List<Quest>();
		completedQuestList = new List<Quest>();
		activeQuestList = new List<Quest>();

		questTitle = GameObject.Find("Titulo");
		questDescription = GameObject.Find("Descripcion");
		questId = GameObject.Find("Id");
		questState = GameObject.Find("Estado");
		questType = GameObject.Find("Tipo");

		position = new Vector3(0.0f, 0.0f, 0.0f);

		// Testing --------------------------------------------------------------------------------------------------
		CreateQuest(0, "Quest de Prueba", "Soy una Quest de prueba", QuestState.ACTIVE, QuestType.UNKNOWN);
		CreateQuest(1, "Quest de Prueba 2", "Soy una Quest de prueba 2", QuestState.ACTIVE, QuestType.MAIN);
		CreateQuest(2, "Quest de Prueba 3", "Soy una Quest de prueba 3", QuestState.ACTIVE, QuestType.SECONDARY);

		for (int i = 0; i < questList.Count; ++i)
        {
			Debug.Log("----------------");
			Debug.Log(questList[i].GetQuestId().ToString());
			Debug.Log(questList[i].GetQuestName());
			Debug.Log(questList[i].GetQuestDescription());
			Debug.Log(questList[i].GetQuestState().ToString());
			Debug.Log(questList[i].GetQuestType().ToString());
        }
		// ----------------------------------------------------------------------------------------------------------
	}
	public void Update()
	{
		questTitle.GetComponent<UIText>().text = activeQuestList[4].GetQuestName().ToString();
		position.Set(0.0f, 0.0f, 0.0f);
		questTitle.GetComponent<Transform2D>().position2D = position;

		questDescription.GetComponent<UIText>().text = activeQuestList[4].GetQuestDescription().ToString();
		position.Set(0.0f, 10.0f, 0.0f);
		questDescription.GetComponent<Transform2D>().position2D = position;

		questId.GetComponent<UIText>().text = activeQuestList[4].GetQuestId().ToString();
		position.Set(0.0f, 20.0f, 0.0f);
		questId.GetComponent<Transform2D>().position2D = position;

		questState.GetComponent<UIText>().text = activeQuestList[4].GetQuestState().ToString();
		position.Set(0.0f, 30.0f, 0.0f);
		questState.GetComponent<Transform2D>().position2D = position;

		questType.GetComponent<UIText>().text = activeQuestList[4].GetQuestType().ToString();
		position.Set(0.0f, 40.0f, 0.0f);
		questType.GetComponent<Transform2D>().position2D = position;
		// Testing --------------------------------------------------------------------------------------------------
		if (Input.GetKey(KeyCode.C) == KeyState.KEY_DOWN)
		{
			ChangeQuestState(activeQuestList[0], QuestState.COMPLETED);
		}

		if (Input.GetKey(KeyCode.D) == KeyState.KEY_DOWN)
		{
			Debug.Log(questList.Count.ToString());
			if (questList.Count > 0)
			{
				RemoveQuest(questList[questList.Count - 1]);
			}
		}
		// ----------------------------------------------------------------------------------------------------------
	}
}