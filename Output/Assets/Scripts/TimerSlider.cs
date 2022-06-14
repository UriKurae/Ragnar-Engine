using System;
using RagnarEngine;

public class TimerSlider : RagnarComponent
{
    float TotalTime = 0;
    Vector3 newBounds;
    Vector3 Newposition;
    GameObject enemy=null;
    float lastX = 100;
    EnemyType typeOfEnemy=0;
    string theAction;
    public void Start()
    {
        Newposition = new Vector3(0, 0, 0);
        newBounds = new Vector3(0, 0, 0);
        gameObject.GetComponent<UIImage>().SetImageGeneralColor(255, 0, 0);
    }
    void SetTimer(float actualTime,Vector3 position)
    {
        float mult = actualTime / TotalTime;
        float newBound = mult * 100;

        float x=lastX - newBound;
        newBounds.Set(newBound, 7, 0);

        //gameObject.GetComponent<Transform>() = position;
        Newposition = Camera.WorldToScreen(position);
        Newposition.y= Newposition.y + 70;
        Newposition.x = Newposition.x-(x/2);
        gameObject.GetComponent<Transform2D>().position2D= Newposition;
        gameObject.GetComponent<Transform2D>().SetSize(newBounds);
    }
    public void getGa(GameObject obj,float time,EnemyType enemyTy,string action)
    {
        enemy = obj;
        typeOfEnemy = enemyTy;
        TotalTime = time;
        theAction=action;
    }

    public void Update()
    {
        if (enemy != null)
        {
            float time=0;
            switch (typeOfEnemy)
            {
                case EnemyType.BASIC:
                    if(theAction== "distractedTimer")
                    {
                        time = enemy.GetComponent<BasicEnemy>().distractedTimer;
                        SetTimer(time, enemy.GetComponent<Transform>().globalPosition);

                    }else if(theAction == "stunnedTimer")
                    {
                        time = enemy.GetComponent<BasicEnemy>().stunnedTimer;
                        SetTimer(time, enemy.GetComponent<Transform>().globalPosition);
                    }
                    else if(theAction == "controlledCooldown")
                    {
                        time = enemy.GetComponent<BasicEnemy>().controlledCooldown;
                        SetTimer(time, enemy.GetComponent<Transform>().globalPosition);
                    }
                    
                    break;
                case EnemyType.TANK:
                    if (theAction == "distractedTimer")
                    {
                        time = enemy.GetComponent<TankEnemy>().distractedTimer;
                        SetTimer(time, enemy.GetComponent<Transform>().globalPosition);
                    }
                    else if (theAction == "stunnedTimer")
                    {
                        time = enemy.GetComponent<TankEnemy>().stunnedTimer;
                        Newposition = enemy.GetComponent<Transform>().globalPosition;
                        Newposition.y = Newposition.y + 40;
                        SetTimer(time, Newposition);
                    }
                    else if (theAction == "controlledCooldown")
                    {
                        time = enemy.GetComponent<TankEnemy>().controlledCooldown;
                        SetTimer(time, enemy.GetComponent<Transform>().globalPosition);
                    }
                    
                    break;
                case EnemyType.UNDISTRACTABLE:
                    if (theAction == "distractedTimer")
                    {
                        time = enemy.GetComponent<UndistractableEnemy>().distractedTimer;
                        SetTimer(time, enemy.GetComponent<Transform>().globalPosition);

                    }
                    else if (theAction == "stunnedTimer")
                    {
                        time = enemy.GetComponent<UndistractableEnemy>().stunnedTimer;
                        Newposition = enemy.GetComponent<Transform>().globalPosition;
                        Newposition.y = Newposition.y + 40;
                        SetTimer(time, Newposition);
                    }
                    else if (theAction == "controlledCooldown")
                    {
                        time = enemy.GetComponent<UndistractableEnemy>().controlledCooldown;
                        SetTimer(time, enemy.GetComponent<Transform>().globalPosition);
                    }
                    break;
            }
            if (time <= 0)
            {
                InternalCalls.Destroy(gameObject);

            }
        }
    }

}