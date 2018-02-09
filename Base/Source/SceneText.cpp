#include "SceneText.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "KeyboardController.h"
#include "MouseController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"
#include "EntityManager.h"

#include "GenericEntity.h"
#include "GroundEntity.h"
#include "TextEntity.h"
#include "SpriteEntity.h"
#include "Light.h"

#include "SceneNode.h"
#include "SceneGraph.h"

#include "Test Dummy\Dummy.h"
#include <iostream>

#include "Waypoint\WaypointManager.h"
#include "State\StateMachineManager.h"
#include "State\EnemyPatrol.h"
#include "State\EnemyChase.h"

//SceneText* SceneText::sInstance = new SceneText(SceneManager::GetInstance());

SceneText::SceneText()
{
}

//SceneText::SceneText(SceneManager* _sceneMgr)
//{
//	_sceneMgr->AddScene("Start", this);
//}

SceneText::~SceneText()
{
	CWaypointManager::GetInstance()->DropInstance();
	CSpatialPartition::GetInstance()->RemoveCamera();
	CSceneGraph::GetInstance()->Destroy();
}

void SceneText::Init()
{
	// Tell the graphics manager to use the shader we just loaded
	//GraphicsManager::GetInstance()->SetActiveShader("default");

	lights[0] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[0]", lights[0]);
	lights[0]->type = Light::LIGHT_DIRECTIONAL;
	lights[0]->position.Set(0, 20, 0);
	lights[0]->color.Set(1, 1, 1);
	lights[0]->power = 1;
	lights[0]->kC = 1.f;
	lights[0]->kL = 0.01f;
	lights[0]->kQ = 0.001f;
	lights[0]->cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0]->cosInner = cos(Math::DegreeToRadian(30));
	lights[0]->exponent = 3.f;
	lights[0]->spotDirection.Set(0.f, 1.f, 0.f);
	lights[0]->name = "lights[0]";

	lights[1] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[1]", lights[1]);
	lights[1]->type = Light::LIGHT_DIRECTIONAL;
	lights[1]->position.Set(1, 1, 0);
	lights[1]->color.Set(1, 1, 0.5f);
	lights[1]->power = 0.4f;
	lights[1]->name = "lights[1]";
	
	// Create the playerinfo instance, which manages all information about the player
	playerInfo = CPlayerInfo::GetInstance();
	playerInfo->SetStartPos("stage1_start");
	playerInfo->Init();

	// Create and attach the camera to the scene
	//camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	camera.Init(playerInfo->GetPos(), playerInfo->GetTarget(), playerInfo->GetUp());
	playerInfo->AttachCamera(&camera);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateAxes("reference");
	MeshBuilder::GetInstance()->GenerateCrossHair("crosshair");
	MeshBuilder::GetInstance()->GenerateQuad("quad", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("quad")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);
	MeshBuilder::GetInstance()->GenerateOBJ("Chair", "OBJ//chair.obj");
	MeshBuilder::GetInstance()->GetMesh("Chair")->textureID = LoadTGA("Image//chair.tga");
	MeshBuilder::GetInstance()->GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	MeshBuilder::GetInstance()->GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	MeshBuilder::GetInstance()->GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 1.f);
	MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kSpecular.Set(0.f, 0.f, 0.f);
	MeshBuilder::GetInstance()->GenerateQuad("GRASS_DARKGREEN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GRASS_DARKGREEN")->textureID = LoadTGA("Image//grass1.tga");
	MeshBuilder::GetInstance()->GenerateQuad("GEO_GRASS_LIGHTGREEN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GEO_GRASS_LIGHTGREEN")->textureID = LoadTGA("Image//grass2.tga");

	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_FRONT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BACK", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_LEFT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_RIGHT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_TOP", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BOTTOM", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_FRONT")->textureID = LoadTGA("Image//SkyBox//hills_ft.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BACK")->textureID = LoadTGA("Image//SkyBox//hills_bk.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_LEFT")->textureID = LoadTGA("Image//SkyBox//hills_lf.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_RIGHT")->textureID = LoadTGA("Image//SkyBox//hills_rt.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_TOP")->textureID = LoadTGA("Image//SkyBox//hills_up.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BOTTOM")->textureID = LoadTGA("Image//SkyBox//hills_dn.tga");
	MeshBuilder::GetInstance()->GenerateRay("laser", 10.f);

	MeshBuilder::GetInstance()->GenerateCube("Dummy_head", Color(0, 0, 0), 1.f);
	MeshBuilder::GetInstance()->GenerateCube("Dummy_body", Color(0, 1, 0), 1.f);
	MeshBuilder::GetInstance()->GenerateCube("Dummy_arm", Color(0, 0, 1), 1.f);

	// FOR ASSIGNMENT
	MeshBuilder::GetInstance()->GenerateOBJ("rock1", "OBJ//rock1.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("rock2", "OBJ//rock2.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("rock3", "OBJ//rock3.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("tree1", "OBJ//tree1.obj");
	MeshBuilder::GetInstance()->GetMesh("tree1")->textureID = LoadTGA("Image//tree.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("tree2", "OBJ//tree2.obj");
	MeshBuilder::GetInstance()->GetMesh("tree2")->textureID = LoadTGA("Image//tree.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("tree3", "OBJ//tree3.obj");
	MeshBuilder::GetInstance()->GetMesh("tree3")->textureID = LoadTGA("Image//tree.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("elephant1", "OBJ//elephant1.obj");
	MeshBuilder::GetInstance()->GetMesh("elephant1")->textureID = LoadTGA("Image//elephant.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("elephant2", "OBJ//elephant2.obj");
	MeshBuilder::GetInstance()->GetMesh("elephant2")->textureID = LoadTGA("Image//elephant.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("elephant3", "OBJ//elephant3.obj");
	MeshBuilder::GetInstance()->GetMesh("elephant3")->textureID = LoadTGA("Image//elephant.tga");

	MeshBuilder::GetInstance()->GenerateQuad("PORTAL", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("PORTAL")->textureID = LoadTGA("Image//portal.tga");

	// Customise the ground entity
	groundEntity = Create::Ground("GRASS_DARKGREEN", "GEO_GRASS_LIGHTGREEN");
	groundEntity->SetPosition(Vector3(0, -10, 0));
	groundEntity->SetScale(Vector3(100.0f, 100.0f, 100.0f));
	groundEntity->SetGrids(Vector3(10.0f, 1.0f, 10.0f));

	//Set up the Spatial Partition and pass it to the entityManager to manage
	CSpatialPartition::GetInstance()->Init(groundEntity->GetScale().x, groundEntity->GetScale().z, 10, 10, 10);
	CSpatialPartition::GetInstance()->SetMesh("GRIDMESH");
	CSpatialPartition::GetInstance()->SetCamera(&camera);
	CSpatialPartition::GetInstance()->SetLevelOfDetails(40000.0f,160000.0f);
	//CSpatialPartition::GetInstance()->SetLevelOfDetails(20.0f, 80.0f);
	EntityManager::GetInstance()->SetSpatialPartition(CSpatialPartition::GetInstance());
	MeshBuilder::GetInstance()->GenerateCube("cubea", Color(1.0f, 1.0f, 1.0f), 1.0f);
	CSpatialPartition::GetInstance()->SetMesh("cubea");
	//Portal = new GenericEntity();
	Portal = Create::Entity("PORTAL", Vector3(0.f, 5.f, -50.f), Vector3(24.f, 24.f, 24.f));
	Portal->SetCollider(true);
	Portal->SetAABB(Vector3(1.f, 1.f, 1.f), Vector3(-1.f, -1.f, -1.f));
	// Create entities into the scene
	MeshBuilder::GetInstance()->GenerateCube("cubeSG", Color(1.f, 0.64f, 0.f), 1.f);
	// Create entities into the scene
	MeshBuilder::GetInstance()->GenerateCube("cubeSG", Color(1.f, 0.64f, 0.f), 1.f);
	// Create a Waypoint inside WaypointManager
	int aWaypoint = CWaypointManager::GetInstance()->AddWaypoint(Vector3(10.0f, 0.0, 50.0f));
	int anotherWaypoint = CWaypointManager::GetInstance()->AddWaypoint(Vector3(10.0f, 0.0f, -50.0f));
	CWaypointManager::GetInstance()->AddWaypoint(anotherWaypoint, Vector3(-50.0f, 0.0f, 0.0f));
	CWaypointManager::GetInstance()->PrintSelf();

	//**************************
	// Initialise the enemy
	theEnemy = new CEnemy();
	theEnemy->Init();
	theEnemy->SetActive(true);
	theEnemy->SetType("Enemy");
	theEnemy->SetMesh(MeshBuilder::GetInstance()->GetMesh("cube"));

	// FSM
	StateMachineManager::GetInstance()->AddState("Enemy", new EnemyPatrol("enemy_patrol"));
	StateMachineManager::GetInstance()->AddState("Enemy", new EnemyChase("enemy_chase"));
	StateMachineManager::GetInstance()->DefaultState(theEnemy);
	StateMachineManager::GetInstance()->AddGameObject(theEnemy);

//	Create::Text3DObject("text", Vector3(0.0f, 0.0f, 0.0f), "DM2210", Vector3(10.0f, 10.0f, 10.0f), Color(0, 1, 1));
	Create::Sprite2DObject("crosshair", Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));

	theSkyBox = Create::SkyBox("SKYBOX_FRONT", "SKYBOX_BACK",
											 "SKYBOX_LEFT", "SKYBOX_RIGHT",
											 "SKYBOX_TOP", "SKYBOX_BOTTOM");

	playerInfo->SetTerrain(groundEntity);
	theEnemy->SetTerrain(groundEntity);
	
	// Setup the 2D entities
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	float fontSize = 25.0f;
	float halfFontSize = fontSize / 2.0f;
	for (int i = 0; i < 3; ++i)
	{
		textObj[i] = Create::Text2DObject("text", Vector3(-halfWindowWidth, -halfWindowHeight + fontSize*i + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f,1.0f,0.0f));
	}
	textObj[0]->SetText("HELLO WORLD");

	countDown = 5.f;
	groundEntity->SetIsDone(false);
	Portal->SetIsDone(false);
}

void SceneText::Update(double dt)
{
	// Update our entities
	EntityManager::GetInstance()->Update(dt);
	StateMachineManager::GetInstance()->Update(dt);

	// THIS WHOLE CHUNK TILL <THERE> CAN REMOVE INTO ENTITIES LOGIC! Or maybe into a scene function to keep the update clean
	if(KeyboardController::GetInstance()->IsKeyDown('1'))
		glEnable(GL_CULL_FACE);
	if(KeyboardController::GetInstance()->IsKeyDown('2'))
		glDisable(GL_CULL_FACE);
	if(KeyboardController::GetInstance()->IsKeyDown('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(KeyboardController::GetInstance()->IsKeyDown('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	if(KeyboardController::GetInstance()->IsKeyDown('5'))
	{
		lights[0]->type = Light::LIGHT_POINT;
	}
	else if(KeyboardController::GetInstance()->IsKeyDown('6'))
	{
		lights[0]->type = Light::LIGHT_DIRECTIONAL;
	}
	else if(KeyboardController::GetInstance()->IsKeyDown('7'))
	{
		lights[0]->type = Light::LIGHT_SPOT;
	}

	if(KeyboardController::GetInstance()->IsKeyDown('I'))
		lights[0]->position.z -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('K'))
		lights[0]->position.z += (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('J'))
		lights[0]->position.x -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('L'))
		lights[0]->position.x += (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('O'))
		lights[0]->position.y -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('P'))
		lights[0]->position.y += (float)(10.f * dt);

	// if the left mouse button was released
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
	{
		cout << "Left Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::RMB))
	{
		cout << "Right Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::MMB))
	{
		cout << "Middle Mouse Button was released!" << endl;
		/*cout << "Loading State2" << endl;
		SceneManager::GetInstance()->SetActiveScene("GameState2");*/
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) != 0.0)
	{
		//cout << "Mouse Wheel has offset in X-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) << endl;
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) != 0.0)
	{
		//cout << "Mouse Wheel has offset in Y-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) << endl;
	}
	// <THERE>
	if (KeyboardController::GetInstance()->IsKeyDown('P'))
	{
		cout << "Loading EndScene" << endl;
		SceneManager::GetInstance()->SetActiveScene("EndSceneState");
	}
	if (KeyboardController::GetInstance()->IsKeyDown('O'))
	{
		cout << "Loading Game2" << endl;
		SceneManager::GetInstance()->SetActiveScene("GameState2");
	}
	if (KeyboardController::GetInstance()->IsKeyReleased('M'))
	{
		CSceneNode *theNode = CSceneGraph::GetInstance()->GetNode(1);
		Vector3 pos = theNode->GetEntity()->GetPosition();
		theNode->GetEntity()->SetPosition(Vector3(pos.x + 50.0f, pos.y, pos.z + 50.0f));
	}
	if (KeyboardController::GetInstance()->IsKeyReleased('N'))
	{
		CSpatialPartition::GetInstance()->PrintSelf();
	}
	if (KeyboardController::GetInstance()->IsKeyReleased('E'))
	{
		CSceneGraph::GetInstance()->GetRoot()->PrintSelf();
	}
	if (KeyboardController::GetInstance()->IsKeyReleased('V'))
	{
		GenericEntity *DummyHead = Create::Asset("Dummy_head", Vector3(0.f, 0.f, 0.f));
		CSceneNode *DummyHNode = CSceneGraph::GetInstance()->AddNode(DummyHead);
		//Math::RandFloatMinMax()
		CUpdateTransformation *mix = new CUpdateTransformation();
		mix->ApplyUpdate(0, 0.1f, 0);
		
		mix->SetSteps(0, 100);
		DummyHNode->SetUpdateTransformation(mix);
		
		
		GenericEntity *DummyBody = Create::Asset("Dummy_body", Vector3(0.f, 0.f, 0.f));
		CSceneNode *DummyBNode = DummyHNode->AddChild(DummyBody);
		DummyBNode->ApplyTranslate(0.f, -1.f, 0.f);
		GenericEntity *Dummy_leftarm = Create::Asset("Dummy_arm", Vector3(0.f, 0.f, 0.f));
		CSceneNode *DummyLANode = DummyBNode->AddChild(Dummy_leftarm);
		DummyLANode->ApplyTranslate(-1.f, 0.f, 0.f);
		GenericEntity *Dummy_rightarm = Create::Asset("Dummy_arm", Vector3(0.f, 0.f, 0.f));
		CSceneNode *DummyRANode = DummyBNode->AddChild(Dummy_rightarm);
		DummyRANode->ApplyTranslate(1.f, 0.f, 0.f);
		CUpdateTransformation *aRotateMtx = new CUpdateTransformation();
		aRotateMtx->ApplyUpdate(1.0f, 1.0f, 0.0f, 0.0f);
		aRotateMtx->SetSteps(-120, 60);
		DummyRANode->SetUpdateTransformation(aRotateMtx);
	}
	if (KeyboardController::GetInstance()->IsKeyReleased('X'))
	{
		int entityposX = Math::RandIntMinMax(-40, 40);
		int entityposZ = Math::RandIntMinMax(-40, 40);

		Vector3 dummypos = Vector3(entityposX, 10, entityposZ);

		TestDummy *Test = Create::dummy_part("Dummy_head",dummypos);
		TestDummy *Test1 = Create::dummy_part("Dummy_body",dummypos +Vector3(0,-1.1f,0));
		TestDummy *Test2 = Create::dummy_part("Dummy_arm", dummypos + Vector3(-1.1f, -1.1f, 0));
		TestDummy *Test3 = Create::dummy_part("Dummy_arm", dummypos + Vector3(1.1f, -1.1f, 0));

		
		CSceneNode *HeadNode = CSceneGraph::GetInstance()->AddNode(Test);
		CSceneNode *BodyNode = HeadNode->AddChild(Test1);
		CSceneNode *LANode = BodyNode->AddChild(Test2);
		CSceneNode *RANode = BodyNode->AddChild(Test3);
	}
	
	if ((playerInfo->GetPos() - Portal ->GetPosition()).LengthSquared() <50.0f)
	{
		cout << "Loading Game2" << endl;
		SceneManager::GetInstance()->SetActiveScene("GameState2");
	}

	// Update the player position and other details based on keyboard and mouse inputs
	playerInfo->Update(dt);

	//camera.Update(dt); // Can put the camera into an entity rather than here (Then we don't have to write this)

	//GraphicsManager::GetInstance()->UpdateLights(dt);

	// Update the 2 text object values. NOTE: Can do this in their own class but i'm lazy to do it now :P
	// Eg. FPSRenderEntity or inside RenderUI for LightEntity
	std::ostringstream ss;
	ss.precision(5);
	float fps = (float)(1.f / dt);
	ss << "FPS: " << fps;
	textObj[1]->SetText(ss.str());

	std::ostringstream ss1;
	ss1.precision(4);
	ss1 << "Player:" << playerInfo->GetPos();
	textObj[2]->SetText(ss1.str());
}

void SceneText::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 3D pipeline then render 3D
	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);
	EntityManager::GetInstance()->Render();

	// Setup 2D pipeline then render 2D
	int halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2;
	int halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2;
	GraphicsManager::GetInstance()->SetOrthographicProjection(-halfWindowWidth, halfWindowWidth, -halfWindowHeight, halfWindowHeight, -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();
	EntityManager::GetInstance()->RenderUI();
}

void SceneText::Exit()
{
	// Save player's position
	CPlayerInfo::GetInstance()->SavePlayerPos();

	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
	playerInfo->DetachCamera();

	if (playerInfo->DropInstance() == false)
	{
#if _DEBUGMODE==1
		cout << "Unable to drop PlayerInfo class" << endl;
#endif
	}

	// Clean the entity manager
	EntityManager::GetInstance()->Clean();
	// Clean the state machine manager
	StateMachineManager::GetInstance()->CleanManager();

	// Delete the lights
	GraphicsManager::GetInstance()->RemoveLight("lights[0]");
	GraphicsManager::GetInstance()->RemoveLight("lights[1]");
}
