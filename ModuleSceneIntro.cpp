#include "Globals.h"
#include "ModulePlayer.h"
#include "PhysVehicle3D.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

#include <stdio.h>


ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
    
    App->player->Enable();
    turboResetTimer = 500.0f;

    startCountDown = 300;
    countdownTickFx = App->audio->LoadFx("Assets/Audio/countdown_1.wav");
    countdownGoFx = App->audio->LoadFx("Assets/Audio/countdown_go.wav");

    /*l.SetPos(1, 30, 65);
    l.Active(true);
    l.ambient = Red;
    l.diffuse = Blue;*/

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
    
    donut.SetVar(2, 8, 50, 50);
    donut.SetPos(1, 30, 65);
    //donut.color.Set(1.0f, 0, 0, 1.0f);

	MapCreation();
    vec3 pos(torusCheckpointList.getFirst()->data->GetTransform().M[12],
             torusCheckpointList.getFirst()->data->GetTransform().M[13],
             torusCheckpointList.getFirst()->data->GetTransform().M[14]);

    // 20 is for outer radius
    Cube* l1 = CreateCube(vec3(pos.x, 12, pos.z - 7), vec3(4, 4, 4), Black, 0, "firstlight");
    lights.add(l1);

    lights.add(CreateCube(vec3(pos.x, 12, pos.z), vec3(4, 4, 4), Black, 0, "secondlight"));
    Cube* l3 = CreateCube(vec3(pos.x, 12, pos.z + 7), vec3(4, 4, 4), Black, 0, "thirdlight");
    lights.add(l3);

    Cube* fStick = CreateCube(vec3(-120.387f, 15, -26.1f), vec3(1, 5, 1), Black, 0, "fstick");
    Cube* sStick = CreateCube(vec3(-120.387f, 15, -19.1f), vec3(1, 5, 1), Black, 0, "sstick");
    Cube* tStick = CreateCube(vec3(-120.387f, 15, -12.1f), vec3(1, 5, 1), Black, 0, "tstick");
    sticksList.add(fStick);
    sticksList.add(sStick);
    sticksList.add(tStick);

    myinit();

    
    fxCheckpoint = App->audio->LoadFx("Assets/Audio/checkpoint.wav");
    fxLapCompleted = App->audio->LoadFx("Assets/Audio/lap.wav");
    fxWin = App->audio->LoadFx("Assets/Audio/you_win.wav");
    fxLose = App->audio->LoadFx("Assets/Audio/you_lose.wav");

    App->audio->PlayMusic("Assets/Audio/nine_thou.ogg");

    laps = 1;
    swapCamera = false;
    lapTime = 0;
    bestLapTime = 43600;
    lastLapTime = 0;
    bestTimeLap = 0;

    return ret;
}

// UnLoad assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	geometryList.clear();
    physBodies.clear();
    checkpointList.clear();
    torusCheckpointList.clear();
    sticksList.clear();
    turbosList.clear();


	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
    //startCountDown -= dt;
    //LOG("%i", startCountDown);
    CountDown(dt);
    
    if (go && laps < 4)
    {
        lapTime += 1 * dt;
        LOG("%f===", lapTime)
    }
    else if (laps == 4)
    {
        FILE* timesFile;
        errno_t err = fopen_s(&timesFile, "Times.txt", "a+");
        if (!err)
        {
            if (bestTimeLap == 2)
                fprintf_s(timesFile, "Your best time was: %.3f seconds at %i st lap\n", bestLapTime, bestTimeLap - 1);
            else if (bestTimeLap == 3)
                fprintf_s(timesFile, "Your best time was: %.3f seconds at %i nd lap\n", bestLapTime, bestTimeLap - 1);
            else if (bestTimeLap == 4)
                fprintf_s(timesFile, "Your best time was: %.3f seconds at %i rd lap\n", bestLapTime, bestTimeLap - 1);

        }

        // Lose and win FX
        if (bestLapTime <= 50.0f && playOnceWinLoseFx)
        {
            playOnceWinLoseFx = false;
            App->audio->PlayFx(fxWin);
        }
        else if(bestLapTime > 50.0f && playOnceWinLoseFx)
        {
            playOnceWinLoseFx = false;
            App->audio->PlayFx(fxLose);
        }
    }

    ResetTurbos(dt);

    display(dt);
	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
    if (body1->IsSensor())
    {
        LOG("Body1: Im a sensor my name is %s", body1->name.GetString());

    }
    else if (body2->IsSensor())
    {
        LOG("Body2: Im a sensor my name is %s", body2->name.GetString());
        
        if (body2->name == "loopsensor1")
        {
            swapCamera = true;
        }

        else if (body2->name == "loopsensor2")
        {
            swapCameraTop = true;
        }

        else if (body2->name == "loopsensor3" || body2->name == "loopsensor4" || body2->name == "loopsensor5" || body2->name == "loopsensor6" || body2->name == "loopsensor7" || body2->name == "loopsensor8" || body2->name == "loopsensor9")
        {
            swapCamera = false;
            swapCameraTop = false;
        }

        if (body2->name == "turbo")
        {
            App->player->turbo = 100.0f;
            body2->name = "turboPicked";
            turbosList.add(body2);
        }
        
        if (body2->name == "startcheckpoint" && checkpointList.count() == 0)
        {
            checkpointList.add(body2);

            // Timer reset
            lastLapTime = lapTime;
            if (lastLapTime < bestLapTime && laps > 1)
            {
                bestLapTime = lastLapTime;
                bestTimeLap = laps;
            }
            lapTime = 0.0f;

            if (laps > 1)
                App->audio->PlayFx(fxLapCompleted);
        }
        else if (body2->name == "secondcheckpoint")
        {
            if (checkpointList.count() > 0)
            {
                // If the first checkpoint is achieved, just add the second one to the list
                if (checkpointList.getLast()->data->name == "startcheckpoint")
                {
                    checkpointList.add(body2);
                    App->audio->PlayFx(fxCheckpoint);
                }
                // If not, teleport the player to the previous checkpoint
                if (checkpointList.getLast()->data->name != body2->name)
                {
                    btVector3 a = body2->GetPosition();
                    App->player->vehicle->SetPos(a.getX(), a.getY(), a.getZ());
                }
                //checkpointList.add(body2);
                torusCheckpointList.atIndex(1)->data->color = Green;
            }
            else
            {
                btVector3 a = body2->GetPosition();
                App->player->vehicle->SetPos(a.getX(), a.getY(), a.getZ());
            }

        }
        else if (body2->name == "thirdcheckpoint")
        {
            if (checkpointList.count() > 0)
            {
                if (checkpointList.getLast()->data->name == "secondcheckpoint")
                {
                    checkpointList.add(body2);
                    App->audio->PlayFx(fxCheckpoint);
                }
                if (checkpointList.getLast()->data->name != body2->name)
                {
                    btVector3 a = body2->GetPosition();
                    App->player->vehicle->SetPos(a.getX(), a.getY(), a.getZ());
                }
                torusCheckpointList.atIndex(2)->data->color = Green;
            }
            else
            {
                btVector3 a = body2->GetPosition();
                App->player->vehicle->SetPos(a.getX(), a.getY(), a.getZ());
            }
        }
        else if (body2->name == "fourthcheckpoint")
        {
            if (checkpointList.count() > 0)
            {
                if (checkpointList.getLast()->data->name == "thirdcheckpoint")
                {
                    checkpointList.add(body2);
                    App->audio->PlayFx(fxCheckpoint);
                }
                if (checkpointList.getLast()->data->name != body2->name)
                {
                    btVector3 a = body2->GetPosition();
                    App->player->vehicle->SetPos(a.getX(), a.getY(), a.getZ());
                }
                torusCheckpointList.atIndex(3)->data->color = Green;
            }
            else
            {
                btVector3 a = body2->GetPosition();
                App->player->vehicle->SetPos(a.getX(), a.getY(), a.getZ());
            }
        }

        if (checkpointList.count() == 4 && body2->name == "startcheckpoint")
        {
            // LAP DONE
            checkpointList.clear();
            laps++;

            p2List_item<Torus*>* restoreColor = torusCheckpointList.getFirst()->next;

            while (restoreColor != nullptr)
            {
                restoreColor->data->color = Blue;
                restoreColor = restoreColor->next;
            }

            LOG("%d", laps);
        }
    }
}

Cube* ModuleSceneIntro::CreateCube(vec3 pos, vec3 size, Color rgb, float mass, SString name, bool isSensor)
{
	Cube* cube = new Cube();
	cube->SetPos(pos.x,pos.y,pos.z);
	cube->size = size;
	cube->color = rgb;
   
	physBodies.add(App->physics->AddBody(*cube, mass, isSensor, name));

	return cube;
}

Cube* ModuleSceneIntro::CreateRamp(vec3 pos, vec3 size, Color rgb, float angle, vec3 pivot, SString name ,float mass, bool isSensor)
{
	Cube* cube = new Cube();
	cube->SetRotation(angle, pivot);
	cube->SetPos(pos.x, pos.y, pos.z);
	cube->size = size;
	cube->color = rgb;
    physBodies.add(App->physics->AddBody(*cube, mass, isSensor, name));
	
	return cube;
}

void ModuleSceneIntro::MapCreation()
{
    // Turbos sensors
    geometryList.add(CreateRamp(vec3(-41.036f, 2.659f, -25.965f), vec3(2.0f, 2.0f, 2.0f), Orange, 45.0f, vec3(1.0f,0.0f,1.0f), "turbo", 0,true));
    geometryList.add(CreateRamp(vec3(220.665f, 2.659f, -139.767f), vec3(2.0f, 2.0f, 2.0f), Orange, 45.0f, vec3(1.0f,0.0f,1.0f), "turbo", 0,true));
    geometryList.add(CreateRamp(vec3(471.305f, 2.659f, -139.767f), vec3(2.0f, 2.0f, 2.0f), Orange, 45.0f, vec3(1.0f,0.0f,1.0f), "turbo", 0,true));
    geometryList.add(CreateRamp(vec3(237.303f, 2.659f, -197.916f), vec3(2.0f, 2.0f, 2.0f), Orange, 45.0f, vec3(1.0f,0.0f,1.0f), "turbo", 0,true));
    geometryList.add(CreateRamp(vec3(-41.036f, 2.659f, -784.513f), vec3(2.0f, 2.0f, 2.0f), Orange, 45.0f, vec3(1.0f,0.0f,1.0f), "turbo", 0,true));

    // Circuit walls
    geometryList.add(CreateCube(vec3(-181.0f, 6.5f, -411.212f), vec3(1.0f, 13.0f, 815.0f), Dark_Grey, 0, "wall1"));
    geometryList.add(CreateCube(vec3(-151.028f, 6.5f, -394.152f), vec3(1.0f, 13.0f, 719.176f), Dark_Grey, 0, "wall2"));

    geometryList.add(CreateCube(vec3(-21.028f, 6.5f, -34.393f), vec3(260.0f, 13.0f,1.0f), Dark_Grey, 0, "wall3"));
    geometryList.add(CreateCube(vec3(-21.033f, 6.5f, -4.0f), vec3(320.0f, 13.0f, 1.0f), Dark_Grey, 0, "wall4"));

    geometryList.add(CreateCube(vec3(108.972f, 6.5f, -94.708f), vec3(1.0f, 13.0f, 120.0f), Dark_Grey, 0, "wall5"));
    geometryList.add(CreateCube(vec3(138.972f, 6.5f, -64.0f), vec3(1.0f, 13.0f, 120.249f), Dark_Grey, 0, "wall6"));

    geometryList.add(CreateCube(vec3(188.9742f, 6.5f, -154.393f), vec3(159.915f, 13.0f, 1.0f), Dark_Grey, 0, "wall7"));
    geometryList.add(CreateCube(vec3(218.972f, 6.5f, -124.0f), vec3(159.969f, 13.0f, 1.0f), Dark_Grey, 0, "wall8"));

    geometryList.add(CreateCube(vec3(269.213f, 6.5f, -169.227f), vec3(1.0f, 13.0f, 30.537f), Dark_Grey, 0, "wall9"));
    geometryList.add(CreateCube(vec3(298.409f, 6.5f, -139.388f), vec3(1.0f, 13.0f, 30.412f), Dark_Grey, 0, "wall10"));

    geometryList.add(CreateCube(vec3(379.085f, 6.5f, -184.393f), vec3(220.666f, 13.0f, 1.0f), Dark_Grey, 0, "wall11"));
    geometryList.add(CreateCube(vec3(375.748f, 6.5f, -154.119f), vec3(155.0f, 13.0f, 1.0f), Dark_Grey, 0, "wall12"));

    geometryList.add(CreateCube(vec3(488.907f, 6.5f, 73.447f), vec3(1.0f, 13.0f, 515.0f), Dark_Grey, 0, "wall13"));
    geometryList.add(CreateCube(vec3(452.223f, 6.5f, 67.09f), vec3(1.0f, 13.0f, 441.031f), Dark_Grey, 0, "wall14"));

    geometryList.add(CreateCube(vec3(359.075f, 6.5f, 288.042f), vec3(187.778f, 13.0f, 1.0f), Dark_Grey, 0, "wall15"));
    geometryList.add(CreateCube(vec3(349.358f, 6.5f, 330.812f), vec3(279.98f, 13.0f, 1.0f), Dark_Grey, 0, "wall16"));

    geometryList.add(CreateCube(vec3(210.847f, 6.5f, 111.2f), vec3(1.0f, 13.0f, 441.031f), Dark_Grey, 0, "wall17"));
    geometryList.add(CreateCube(vec3(264.689f, 6.5f, 93.014f), vec3(1.0f, 13.0f, 389.896f), Dark_Grey, 0, "wall18"));

    geometryList.add(CreateCube(vec3(205.705f, 6.5f, -335.4f), vec3(1.0f, 13.0f, 361.872f), Dark_Grey, 0, "wall19"));
    geometryList.add(CreateCube(vec3(268.817f, 6.5f, -347.152f), vec3(1.0f, 13.0f, 325.405f), Dark_Grey, 0, "wall20"));

    geometryList.add(CreateCube(vec3(124.496f, 6.5f, -688.935f), vec3(1.0f, 13.0f, 129.394f), Dark_Grey, 0, "wall21"));
    geometryList.add(CreateCube(vec3(194.752f, 6.5f, -723.446f), vec3(1.0f, 13.0f, 192.205f), Dark_Grey, 0, "wall22"));

    geometryList.add(CreateCube(vec3(-13.058f, 6.5f, -753.415f), vec3(276.0f, 13.0f, 1.0f), Dark_Grey, 0, "wall23"));
    geometryList.add(CreateCube(vec3(6.763f, 6.5f, -818.984f), vec3(376.0f, 13.0f, 1.0f), Dark_Grey, 0, "wall24"));

    // Checkpoints for the map
    Cube* checkpoint1 = CreateCube(vec3(-120.387f, 1.904f, -18.939f), vec3(1.0f, 3.53f, 28.847f), White, 0, "startcheckpoint", true);
    Cube* checkpoint2 = CreateCube(vec3(398.606f, 1.904f, -169.176f), vec3(1.0f, 3.53f, 28.847f), White, 0, "secondcheckpoint", true);
    Cube* checkpoint3 = CreateCube(vec3(237.974f, 1.904f, 278.205f), vec3(51.939f, 3.53f, 2.399f), White, 0, "thirdcheckpoint", true);
    Cube* checkpoint4 = CreateCube(vec3(159.448f, 1.904f, -736.108f), vec3(69.648f, 3.53f, 2.399f), White, 0, "fourthcheckpoint", true);

    geometryList.add(checkpoint1);
    geometryList.add(checkpoint2);
    geometryList.add(checkpoint3);
    geometryList.add(checkpoint4);

    Torus* checkp1 = new Torus(5, 20, 40, 40);
    checkp1->SetPos(-120.387f, 1.904f, -19.1f);
    checkp1->SetRotation(90, vec3(0, 1, 0));
    checkp1->color = Grey;
    torusCheckpointList.add(checkp1);

    Torus* checkp2 = new Torus(5, 20, 40, 40);
    checkp2->SetPos(398.606f, 1.904f, -169.176f);
    checkp2->SetRotation(90, vec3(0, 1, 0));
    checkp2->color = Blue;
    torusCheckpointList.add(checkp2);

    Torus* checkp3 = new Torus(9, 36, 40, 40);
    checkp3->SetPos(237.974f, 1.904f, 278.205f);
    checkp3->color = Blue;
    torusCheckpointList.add(checkp3);

    Torus* checkp4 = new Torus(11.5, 46, 40, 40);
    checkp4->SetPos(159.448f, 1.904f, -736.108f);
    checkp4->color = Blue;
    torusCheckpointList.add(checkp4);


    // Circuit Ramps
    geometryList.add(CreateRamp(vec3(237.65f, 13.418f, -49.115f), vec3(48.89f, 57.7f, 1.0f), Red, -60.0f, vec3(1, 0, 0), "firstramp", 0));
    geometryList.add(CreateRamp(vec3(237.282f, 27.814f, -138.095f), vec3(47.0f, 1.082f, 128.9f), Red, 0, vec3(1, 0, 0), "secondramp", 0));
    geometryList.add(CreateRamp(vec3(237.551f, 13.049f, -227.619f), vec3(47.549f, 58.977f, 1.0f), Red, 60.0f, vec3(1, 0, 0), "thirdramp", 0));
    
    geometryList.add(CreateRamp(vec3(237.65f, 10, -531.34f), vec3(55.445, 30, 2), Black, 0, vec3(1, 0, 0), "loopsensor1", 0, true));
    geometryList.add(CreateRamp(vec3(200.0f, 100, -550.34f), vec3(60, 20, 2), Black, 0, vec3(1, 0, 0), "loopsensor2", 0, true));
    geometryList.add(CreateRamp(vec3(162.46f, 10, -621.843f), vec3(70.707f, 30, 2), Black, 353.537f, vec3(1, 0, 0), "loopsensor3", 0, true));
    geometryList.add(CreateRamp(vec3(271.46f, 30, -570.0f), vec3(5.0f, 170, 150), Black, 353.537f, vec3(1, 0, 0), "loopsensor4", 0, true));
    geometryList.add(CreateRamp(vec3(125.46f, 30, -570.0f), vec3(5.0f, 170, 150), Black, 353.537f, vec3(1, 0, 0), "loopsensor5", 0, true));
    geometryList.add(CreateRamp(vec3(195.46f, 110, -570.843f), vec3(160.707f, 5.0f, 170), Black, 353.537f, vec3(1, 0, 0), "loopsensor6", 0, true));
    geometryList.add(CreateRamp(vec3(235.46f, 30, -620.0f), vec3(75, 170, 5.0f), Black, 353.537f, vec3(1, 0, 0), "loopsensor7", 0, true));
    geometryList.add(CreateRamp(vec3(168.46f, 30, -490.0f), vec3(75, 170, 5.0f), Black, 353.537f, vec3(1, 0, 0), "loopsensor8", 0, true));
    geometryList.add(CreateRamp(vec3(235.46f, 0, -515.0f), vec3(75, 57, 5.0f), Black, 353.537f, vec3(1, 0, 0), "loopsensor9", 0, true));

    // Wall to prevent rats from cheating
    geometryList.add(CreateCube(vec3(159.575f, 1.853f, -626.894f), vec3(71.223f, 3.599f, 1.0f), Dark_Grey, 0, "wallLoop"));

    // Loop Geometry
    geometryList.add(CreateRamp(vec3(231.65f, 0.1f,-538.34f), vec3(67.319f, 20.333f, 1.0f), Red, -87.0f, vec3(1, 0, 0), "Ramp", 0));
    geometryList.add(CreateRamp(vec3(231.879f,1.897f,-553.9f), vec3(67.319f, 0.953f, 13.055f), Red, 14.018f, vec3(1, 0, 0), "Ramp", 0));
    geometryList.add(CreateRamp(vec3(230.907f,5.594f,-565.905f), vec3(67.319f, 0.953f, 13.055f), Red, 20.258f, vec3(1, 0, 0), "Ramp", 0));
    geometryList.add(CreateRamp(vec3(230.004f,11.401f,-576.966f), vec3(67.319f, 0.953f, 13.055f), Red, 37.067f, vec3(1, 0, 0), "Ramp", 0));
    geometryList.add(CreateRamp(vec3(228.828f,19.481f,-586.252f), vec3(67.319f, 0.953f, 13.055f), Red, 45.018f, vec3(1, 0, 0), "Ramp", 0));
    geometryList.add(CreateRamp(vec3(227.432f,29.843f,-593.885f), vec3(67.319f, 0.953f, 13.055f), Red, 62.58f, vec3(1, 0, 0), "Ramp", 0));
    geometryList.add(CreateRamp(vec3(225.969f,41.755f,-598.69f), vec3(67.319f, 0.953f, 13.055f), Red, 74.222f, vec3(1, 0, 0), "Ramp", 0));
    geometryList.add(CreateRamp(vec3(224.359f,53.108f,-601.199f), vec3(67.319f, 0.953f, 13.055f), Red, 81.058f, vec3(1, 0, 0), "Ramp", 0));
    geometryList.add(CreateRamp(vec3(222.759f,65.954f,-601.964f), vec3(67.319f, 0.953f, 13.055f), Red, 93.239f, vec3(1, 0, 0), "Ramp", 0));
    geometryList.add(CreateRamp(vec3(221.234f,78.35f,-599.545f), vec3(67.319f, 0.953f, 13.055f), Red, 109.016f, vec3(1, 0, 0), "Ramp", 0));
    geometryList.add(CreateRamp(vec3(219.914f,89.779f,-593.405f), vec3(67.319f, 0.953f, 13.055f), Red, 126.834f, vec3(1, 0, 0), "Ramp", 0));
    geometryList.add(CreateRamp(vec3(217.189f,99.005f,-584.629f), vec3(67.319f, 0.953f, 13.055f), Red, 140.501f, vec3(1, 0, 0), "Ramp", 0));
    geometryList.add(CreateRamp(vec3(213.848f,104.901f,-573.774f), vec3(67.319f, 0.953f, 13.055f), Red, 159.895f, vec3(1, 0, 0), "Ramp", 0));
    geometryList.add(CreateRamp(vec3(210.307f,107.897f,-561.694f), vec3(67.319f, 0.953f, 13.055f), Red, 170.994f, vec3(1, 0, 0), "Ramp", 0));
    geometryList.add(CreateRamp(vec3(204.38f,108.484f,-549.12f), vec3(79.457f, 0.953f, 13.055f), Red, 182.879f, vec3(1, 0, 0), "Ramp", 0));
    geometryList.add(CreateRamp(vec3(199.333f,106.716f,-537.087f), vec3(79.457f, 0.953f, 13.055f), Red, 195.692f, vec3(1, 0, 0), "Ramp", 0));
    geometryList.add(CreateRamp(vec3(187.28f,102.132f,-525.792f), vec3(70.707f, 0.953f, 13.055f), Red, 207.885f, vec3(1, 0, 0), "Ramp", 0));
    geometryList.add(CreateRamp(vec3(184.406f,95.484f,-515.291f), vec3(70.707f, 0.953f, 13.055f), Red, 217.328f, vec3(1, 0, 0), "Ramp", 0));
    geometryList.add(CreateRamp(vec3(182.464f,85.66f,-507.133f), vec3(70.707f, 0.953f, 13.055f), Red, 243.023f, vec3(1, 0, 0), "Ramp", 0));
    geometryList.add(CreateRamp(vec3(180.26f,73.535f,-503.602f), vec3(70.707f, 0.953f, 13.055f), Red, 265.401f, vec3(1, 0, 0), "Ramp", 0));
    geometryList.add(CreateRamp(vec3(178.218f,60.588f,-503.602f), vec3(70.707f, 0.953f, 13.055f), Red, 275.225f, vec3(1, 0, 0), "Ramp", 0));
    geometryList.add(CreateRamp(vec3(177.014f,48.039f,-506.307f), vec3(70.707f, 0.953f, 13.055f), Red, 290.753f, vec3(1, 0, 0), "Ramp", 0));
    geometryList.add(CreateRamp(vec3(175.114f,36.793f,-512.918f), vec3(70.707f, 0.953f, 13.055f), Red, 310.139f, vec3(1, 0, 0), "Ramp", 0));
    geometryList.add(CreateRamp(vec3(171.147f,27.66f,-521.768f), vec3(70.707f, 0.953f, 13.055f), Red, 315.61f, vec3(1, 0, 0), "Ramp", 0));
    geometryList.add(CreateRamp(vec3(169.141f,19.813f,-532.305f), vec3(70.707f, 0.953f, 13.055f), Red, 336.008f, vec3(1, 0, 0), "Ramp", 0));
    geometryList.add(CreateRamp(vec3(166.531f,15.518f,-544.363f), vec3(70.707f, 0.953f, 13.055f), Red, 344.502f, vec3(1, 0, 0), "Ramp", 0));
    geometryList.add(CreateRamp(vec3(162.573f,12.638f,-557.217f), vec3(70.707f, 0.953f, 13.055f), Red, 351.625f, vec3(1, 0, 0), "Ramp", 0));
    geometryList.add(CreateRamp(vec3(159.46f,10.819f,-569.927f), vec3(70.707f, 0.953f, 13.055f), Red, 355.566f, vec3(1, 0, 0), "Ramp", 0));
    geometryList.add(CreateRamp(vec3(159.46f,9.465f,-583.044f), vec3(70.707f, 0.953f, 13.055f), Red, 352.87f, vec3(1, 0, 0), "Ramp", 0));
    geometryList.add(CreateRamp(vec3(159.46f,7.854f,-595.88f), vec3(70.707f, 0.953f, 13.055f), Red, 352.906f, vec3(1, 0, 0), "Ramp", 0));
    geometryList.add(CreateRamp(vec3(159.46f,5.971f,-608.888f), vec3(70.707f, 0.953f, 13.055f), Red, 353.537f, vec3(1, 0, 0), "Ramp", 0));
    geometryList.add(CreateRamp(vec3(159.46f,4.109f,-621.843f), vec3(70.707f, 0.953f, 13.055f), Red, 353.537f, vec3(1, 0, 0), "Ramp", 0));    
}

void ModuleSceneIntro::display(float dt)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, texture[1]);

    glDisable(GL_TEXTURE_2D);

    Plane p(0, 1, 0, 0);
    p.axis = true;
    //p.Render();

    //l.Render();

    p2List_item<Cube*>* itemCubes = geometryList.getFirst();

    p2List_item<PhysBody3D*>* itemBodies = physBodies.getFirst();

    while (itemBodies != nullptr && itemCubes != nullptr)
    {
        if (itemBodies->data->IsSensor() != true || itemBodies->data->name == "turbo")
        {
            itemCubes->data->Render();
        }
        itemBodies->data->GetTransform(&itemCubes->data->transform);
        itemCubes = itemCubes->next;
        itemBodies = itemBodies->next;
    }

    itemCubes = geometryList.getFirst();
    itemBodies = physBodies.getFirst();
    while (itemBodies != nullptr && itemCubes != nullptr)
    {
        if (itemBodies->data->name == "turbo")
        {
            itemCubes->data->transform.rotate(angleTurbo, vec3(0.0f, 1.0f, 1.0f));
        }
        itemCubes = itemCubes->next;
        itemBodies = itemBodies->next;
    }
    angleTurbo += 100.0f * dt;

    itemCubes = lights.getFirst();
    while (itemCubes != nullptr)
    {
        itemCubes->data->Render();
        itemCubes = itemCubes->next;
    }

    p2List_item<Torus*>* t = torusCheckpointList.getFirst();
    while (t != nullptr)
    {
        t->data->Render();
        t = t->next;
    }

    p2List_item<Cube*>* c = sticksList.getFirst();
    while (c)
    {
        c->data->Render();
        c = c->next;
    }

    donut.Render();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 0.0);

    glVertex3f(-TEX_SIZE, 0.0, TEX_SIZE); // TEX_SIZE is image size

    glTexCoord2f(1.0, 0.0);

    glVertex3f(TEX_SIZE, 0.0, TEX_SIZE);

    glTexCoord2f(1.0, 1.0);

    glVertex3f(TEX_SIZE, 0.0, -TEX_SIZE);

    glTexCoord2f(0.0, 1.0);

    glVertex3f(-TEX_SIZE, 0.0, -TEX_SIZE);

    glDisable(GL_TEXTURE_2D);

    glEnd();

    //glutSwapBuffers();

}

void ModuleSceneIntro::CountDown(float dt)
{
    if (startCountDown > -200.0f) startCountDown -= 100 * dt;

    if (startCountDown <= -200.0f)
    {
        p2List_item<Cube*>* it = lights.getFirst();
        while (it != nullptr)
        {
            it->data->color = Black;
            it = it->next;
        }
    }
    else if (startCountDown <= 0.0f && go == false)
    {
        p2List_item<Cube*>* it = lights.getFirst();
        while (it != nullptr)
        {
            it->data->color = Green;
            it = it->next;
        }
        App->audio->PlayFx(countdownGoFx);
        go = true;
    }
    else if (startCountDown <= 100 && third == false)
    {
        lights.atIndex(2)->data->color = Red;
        App->audio->PlayFx(countdownTickFx);
        third = true;
    }
    else if (startCountDown <= 200 && second == false)
    {
        lights.atIndex(1)->data->color = Red;
        App->audio->PlayFx(countdownTickFx);
        second = true;
    }
    else if (startCountDown <= 300 && first == false)
    {
        lights.atIndex(0)->data->color = Red;
        App->audio->PlayFx(countdownTickFx);
        first = true;
    }

}

void ModuleSceneIntro::ResetTurbos(float dt)
{
    if (turbosList.count() > 0)
    {
        turboResetTimer -= 100.0f * dt;

        if (turboResetTimer <= 0.0f)
        {
            if (turbosList.getFirst() != nullptr)
            {
                p2List_item<PhysBody3D*>* item = turbosList.getFirst();
                item->data->name = "turbo";
                turbosList.del(item);
                turboResetTimer = 500.0f;
            }
        }
    }

}

void ModuleSceneIntro::ResetScene()
{
    turbosList.clear();
    turboResetTimer = 500.0f;
    angleTurbo = 0.0f;

    laps = 1;

    startCountDown = 300.0f;
    first = false;
    second = false;
    third = false;
    go = false;
}

void ModuleSceneIntro::makeCheckImage(void)
{

    int i, j, c;

    for (i = 0; i < checkImageWidth; i++)
    {

        for (j = 0; j < checkImageHeight; j++)
        {

            c = ((((i & 0x8) == 0) ^ ((j & 0x8) == 0))) * 255;

            checkImage[i][j][0] = (GLubyte)c;

            checkImage[i][j][1] = (GLubyte)c;

            checkImage[i][j][2] = (GLubyte)c;

        }
    }
}

int ModuleSceneIntro::ImageLoad(char* filename, Image* image)
{

    FILE* file;

    unsigned long size; // size of the image in bytes.

    unsigned long i; // standard counter.

    unsigned short int planes; // number of planes in image (must be 1)

    unsigned short int bpp; // number of bits per pixel (must be 24)

    char temp; // temporary color storage for bgr-rgb conversion.

    // make sure the file is there.

    if ((file = fopen(filename, "rb")) == NULL)
    {

        printf("File Not Found : %s\n", filename);

        return 0;

    }

    // seek through the bmp header, up to the width/height:

    fseek(file, 18, SEEK_CUR);

    // read the width

    if ((i = fread(&image->sizeX, 4, 1, file)) != 1)
    {

        printf("Error reading width from %s.\n", filename);

        return 0;

    }

    //printf("Width of %s: %lu\n", filename, image->sizeX);

    // read the height

    if ((i = fread(&image->sizeY, 4, 1, file)) != 1)
    {

        printf("Error reading height from %s.\n", filename);

        return 0;

    }

    //printf("Height of %s: %lu\n", filename, image->sizeY);

    // calculate the size (assuming 24 bits or 3 bytes per pixel).

    size = image->sizeX * image->sizeY * 3;

    // read the planes

    if ((fread(&planes, 2, 1, file)) != 1)
    {

        printf("Error reading planes from %s.\n", filename);

        return 0;

    }
   
    if (planes != 1)
    {

        printf("Planes from %s is not 1: %u\n", filename, planes);

        return 0;

    }

    // read the bitsperpixel
    
    if ((i = fread(&bpp, 2, 1, file)) != 1)
    {

        printf("Error reading bpp from %s.\n", filename);

        return 0;

    }

   
    if (bpp != 24)
    {

        printf("Bpp from %s is not 24: %u\n", filename, bpp);

        return 0;

    }

    // seek past the rest of the bitmap header.

    fseek(file, 24, SEEK_CUR);

    // read the data.

    image->data = (char*)malloc(size);

    if (image->data == NULL)
    {

        printf("Error allocating memory for color-corrected image data");

        return 0;

    }

    if ((i = fread(image->data, size, 1, file)) != 1)
    {

        printf("Error reading image data from %s.\n", filename);

        return 0;

    }

    for (i = 0; i < size; i += 3) // reverse all of the colors. (bgr -> rgb)
    {

        temp = image->data[i];

        image->data[i] = image->data[i + 2];

        image->data[i + 2] = temp;

    }

    // we're done.

    return 1;

}


Image* ModuleSceneIntro::loadTexture()
{

    Image* image1;

    // allocate space for texture

    image1 = (Image*)malloc(sizeof(Image));

    if (image1 == NULL)
    {

        printf("Error allocating space for image");

        exit(0);

    }

    if (!ImageLoad("Assets/Textures/asphalt.bmp", image1))
    {

        exit(1);

    }

    return image1;

}



void ModuleSceneIntro::myinit(void)
{

    glClearColor(0.62, 0.88, 0.99, 0.0); // World color

    glEnable(GL_DEPTH_TEST);

    glDepthFunc(GL_LESS);

    Image* image1 = loadTexture();

    if (image1 == NULL)
    {

        printf("Image was not returned from loadTexture\n");

        exit(0);

    }

    makeCheckImage();

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Create Texture

    glGenTextures(2, texture);

    glBindTexture(GL_TEXTURE_2D, texture[0]);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0,

        GL_RGB, GL_UNSIGNED_BYTE, image1->data);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    glBindTexture(GL_TEXTURE_2D, texture[1]);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, checkImageWidth,

        checkImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, &checkImage[0][0][0]);

    glEnable(GL_TEXTURE_2D);

    glShadeModel(GL_FLAT);
}