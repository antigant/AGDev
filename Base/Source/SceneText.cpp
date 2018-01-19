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
#include "SkyBox/SkyBoxEntity.h"

#include "SceneNode.h"
#include "SceneGraph.h"

#include "Test Dummy\Dummy.h"
#include <iostream>
using namespace std;

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
	CSceneGraph::GetInstance()->Destroy();
}

void SceneText::Init()
{
	//currProg = GraphicsManager::GetInstance()->LoadShader("default", "Shader//Texture.vertexshader", "Shader//Texture.fragmentshader");
	//
	//// Tell the shader program to store these uniform locations
	//currProg->AddUniform("MVP");
	//currProg->AddUniform("MV");
	//currProg->AddUniform("MV_inverse_transpose");
	//currProg->AddUniform("material.kAmbient");
	//currProg->AddUniform("material.kDiffuse");
	//currProg->AddUniform("material.kSpecular");
	//currProg->AddUniform("material.kShininess");
	//currProg->AddUniform("lightEnabled");
	//currProg->AddUniform("numLights");
	//currProg->AddUniform("lights[0].type");
	//currProg->AddUniform("lights[0].position_cameraspace");
	//currProg->AddUniform("lights[0].color");
	//currProg->AddUniform("lights[0].power");
	//currProg->AddUniform("lights[0].kC");
	//currProg->AddUniform("lights[0].kL");
	//currProg->AddUniform("lights[0].kQ");
	//currProg->AddUniform("lights[0].spotDirection");
	//currProg->AddUniform("lights[0].cosCutoff");
	//currProg->AddUniform("lights[0].cosInner");
	//currProg->AddUniform("lights[0].exponent");
	//currProg->AddUniform("lights[1].type");
	//currProg->AddUniform("lights[1].position_cameraspace");
	//currProg->AddUniform("lights[1].color");
	//currProg->AddUniform("lights[1].power");
	//currProg->AddUniform("lights[1].kC");
	//currProg->AddUniform("lights[1].kL");
	//currProg->AddUniform("lights[1].kQ");
	//currProg->AddUniform("lights[1].spotDirection");
	//currProg->AddUniform("lights[1].cosCutoff");
	//currProg->AddUniform("lights[1].cosInner");
	//currProg->AddUniform("lights[1].exponent");
	//currProg->AddUniform("colorTextureEnabled");
	//currProg->AddUniform("colorTexture");
	//currProg->AddUniform("textEnabled");
	//currProg->AddUniform("textColor");
	//
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

	//currProg->UpdateInt("numLights", 1);
	//currProg->UpdateInt("textEnabled", 0);
	
	// Create the playerinfo instance, which manages all information about the player
	playerInfo = CPlayerInfo::GetInstance();
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
	
	GenericEntity *rock = Create::Entity("rock1", Vector3(-20.f, -10.f, 50.f), Vector3(4.f, 4.f, 4.f));
	rock->SetCollider(true);
	rock->SetAABB(Vector3(1.f, 1.f, 1.f), Vector3(-1.f, -1.f, -1.f));
	rock->InitLOD("rock1", "rock2", "rock3");

	GenericEntity *tree = Create::Entity("tree1", Vector3(30, -10.f, 10.f), Vector3(4.f, 4.f, 4.f));
	tree->SetCollider(true);
	tree->SetAABB(Vector3(1.f, 1.f, 1.f), Vector3(-1.f, -1.f, -1.f));
	tree->InitLOD("tree1", "tree2", "tree3");

	GenericEntity *elephant = Create::Entity("elephant1", Vector3(-40.f, -10.f, -20.f), Vector3(5.f, 5.f, 5.f));
	elephant->SetCollider(true);
	elephant->SetAABB(Vector3(1.f, 1.f, 1.f), Vector3(-1.f, -1.f, -1.f));
	elephant->InitLOD("elephant1", "elephant2", "elephant3");

	// Create entities into the scene
	MeshBuilder::GetInstance()->GenerateCube("cubeSG", Color(1.f, 0.64f, 0.f), 1.f);

	Create::Entity("reference", Vector3(0.0f, 0.0f, 0.0f)); // Reference
	Create::Entity("lightball", Vector3(lights[0]->position.x, lights[0]->position.y, lights[0]->position.z)); // Lightball
	
	GenericEntity* aCube = Create::Entity("cube", Vector3(-20.0f, 0.0f, -20.0f));
	aCube->SetCollider(true);
	aCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	aCube->InitLOD("cube", "cone", "cubeSG");
	//Add the pointer to this new entity to the Scene Graph
	CSceneNode* theNode = CSceneGraph::GetInstance()->AddNode(aCube);
	if (theNode == NULL)
		cout << "EntityManager::AddEntity: Unable to add to scene graph!" << endl;

	GenericEntity* anotherCube = Create::Entity("cube", Vector3(-20.0f, 1.1f, -20.0f));
	anotherCube->SetCollider(true);
	anotherCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	CSceneNode* anotherNode = theNode->AddChild(anotherCube);
	if (anotherNode == NULL)
		cout << "EntityManager::AddEntity: Unable to add to scene graph!" << endl;
	//-----------------------------------------------------------------------------------------------------
	// Scene Graph topic 2nd week -> for applying transformation
	GenericEntity *baseCube = Create::Asset("cube", Vector3(0.f, 0.f, 0.f));
	CSceneNode *baseNode = CSceneGraph::GetInstance()->AddNode(baseCube);

	CUpdateTransformation *baseMtx = new CUpdateTransformation();
	baseMtx->ApplyUpdate(0.01f, 0.0f, 0.0f);
	baseMtx->SetSteps(-30, 30);
	baseNode->SetUpdateTransformation(baseMtx);

	GenericEntity *childCube = Create::Asset("cubeSG", Vector3(0.f, 0.f, 0.f));
	CSceneNode *childNode = baseNode->AddChild(childCube);
	childNode->ApplyTranslate(0.f, 1.f, 0.f);

	GenericEntity *grandchildCube = Create::Asset("cubeSG", Vector3(0.f, 0.f, 0.f));
	CSceneNode *grandchildNode = childNode->AddChild(grandchildCube);
	grandchildNode->ApplyTranslate(0.f, 0.f, 1.f);
	CUpdateTransformation *aRotateMtx = new CUpdateTransformation();
	aRotateMtx->ApplyUpdate(1.0f, 0.0f, 0.0f, 1.f);
	aRotateMtx->SetSteps(-120, 60);
	grandchildNode->SetUpdateTransformation(aRotateMtx);
	//***********************************************************************************Dummy Entity

	//GenericEntity *DummyHead = Create::Entity("Dummy_head", Vector3(0.f, 10.f, 0.f));
	//CSceneNode *DummyHNode = CSceneGraph::GetInstance()->AddNode(DummyHead);
	//DummyHNode->ApplyTranslate(0.f, 10.f, 0.f);
	//DummyHead->SetCollider(true);
	//DummyHead->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	//GenericEntity *DummyBody = Create::Entity("Dummy_body", Vector3(0.f, 5.0f, 0.f));
	//CSceneNode *DummyBNode = DummyHNode->AddChild(DummyBody);
	//DummyBNode->ApplyTranslate(0.f, -1.1f, 0.f);
	//DummyBody->SetCollider(true);
	//DummyBody->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	////GenericEntity *Dummy_leftarm = Create::Entity("Dummy_arm", Vector3(0.f, 0.f, 0.f));
	////CSceneNode *DummyLANode = DummyBNode->AddChild(Dummy_leftarm);
	////DummyLANode->ApplyTranslate(-1.f, 0.f, 0.f);
	////GenericEntity *Dummy_rightarm = Create::Entity("Dummy_arm", Vector3(0.f, 0.f, 0.f));
	////CSceneNode *DummyRANode = DummyBNode->AddChild(Dummy_rightarm);
	////DummyRANode->ApplyTranslate(1.f, 0.f, 0.f);
	//CUpdateTransformation *aanotherRotateMtx = new CUpdateTransformation();
	//aanotherRotateMtx->ApplyUpdate(1.0f, 1.0f, 0.0f, 0.0f);
	//aanotherRotateMtx->SetSteps(-120, 60);
	////DummyRANode->SetUpdateTransformation(aRotateMtx);
	//DummyBNode->SetUpdateTransformation(aanotherRotateMtx);


	GenericEntity *HeadDummycube = Create::Entity("cube", Vector3(-10.0f, 1.1f, -20.0f));
	HeadDummycube->SetCollider(true);
	HeadDummycube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	CSceneNode *HeadDummyNode = CSceneGraph::GetInstance()->AddNode(HeadDummycube);

	GenericEntity *BodyDummyCube = Create::Entity("cube", Vector3(-10.0f, 0.0f , -20.0f));
	BodyDummyCube->SetCollider(true);
	BodyDummyCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	CSceneNode* BodyDummyNode = HeadDummyNode->AddChild(BodyDummyCube);
	
	GenericEntity *RightArmDummyCube = Create::Entity("cube", Vector3(-11.1f, 0.0f, -20.0f));
	RightArmDummyCube->SetCollider(true);
	RightArmDummyCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	CSceneNode *RightArmDummyNode = BodyDummyNode->AddChild(RightArmDummyCube);

	GenericEntity *LeftArmDummyCube = Create::Entity("cube", Vector3(-8.9f, 0.0f, -20.0f));
	LeftArmDummyCube->SetCollider(true);
	LeftArmDummyCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	CSceneNode *LeftArmDummyNode = BodyDummyNode->AddChild(LeftArmDummyCube);


		

	//GenericEntity *childCube = Create::Asset("cubeSG", Vector3(0.f, 0.f, 0.f));
	//CSceneNode *childNode = baseNode->AddChild(childCube);
	//childNode->ApplyTranslate(0.f, 1.f, 0.f);

	//GenericEntity *grandchildCube = Create::Asset("cubeSG", Vector3(0.f, 0.f, 0.f));
	//CSceneNode *grandchildNode = childNode->AddChild(grandchildCube);
	//grandchildNode->ApplyTranslate(0.f, 0.f, 1.f);
	//CUpdateTransformation *aRotateMtx = new CUpdateTransformation();
	//aRotateMtx->ApplyUpdate(1.0f, 0.0f, 0.0f, 1.f);
	//aRotateMtx->SetSteps(-120, 60);
	//grandchildNode->SetUpdateTransformation(aRotateMtx);

	//GenericEntity* DummyB = Create::Entity("cube", Vector3(-20.0f, -1.1f, -20.0f));
	//DummyB->SetCollider(true);
	//DummyB->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	//CSceneNode *DummyBN = DummyN->AddChild(DummyB);
	//GenericEntity *DummyL = Create::Entity("Dummy_arm", Vector3(-21.0f, -1.1f, -20.0f));
	//DummyL->SetCollider(true);
	//DummyL->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	//CSceneNode *DummyLN = DummyBN->AddChild(DummyL);
	//
	//GenericEntity *DummyR = Create::Entity("Dummy_arm", Vector3(-19.0f,-1.1f, -20.0f));
	//DummyR->SetCollider(true);
	//DummyR->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	//CSceneNode *DummyRN = DummyBN->AddChild(DummyR);
	//DummyRANode->ApplyTranslate(1.f, 0.f, 0.f);
	/*CUpdateTransformation *aRotateMtx = new CUpdateTransformation();
	aRotateMtx->ApplyUpdate(1.0f, 1.0f, 0.0f, 0.0f);
	aRotateMtx->SetSteps(-120, 60);
	DummyRANode->SetUpdateTransformation(aRotateMtx);*/
	//GenericEntity *weapon = Create::Asset("cone", Vector3(0.f, 0.f, 0.f));
	//CSceneNode *weaponRNode = DummyRANode->AddChild(weapon);

	//**************************
	// Initialise the enemy
	theEnemy = new CEnemy();
	theEnemy->Init();

//	Create::Text3DObject("text", Vector3(0.0f, 0.0f, 0.0f), "DM2210", Vector3(10.0f, 10.0f, 10.0f), Color(0, 1, 1));
	Create::Sprite2DObject("crosshair", Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));

	SkyBoxEntity* theSkyBox = Create::SkyBox("SKYBOX_FRONT", "SKYBOX_BACK",
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
}

void SceneText::Update(double dt)
{
	// Update our entities
	EntityManager::GetInstance()->Update(dt);
	/*if (true)
	{

	}*/
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
	//*weaponRNode = DummyRANode->AddChild(weapon);
		//CSceneGraph ->GetRoot();
	/*	cout << CSceneGraph::GetInstance()->GetNumOfNode() << endl;
		cout<<CSceneGraph::GetInstance()->theRoot << endl;*/
		CSceneGraph::GetInstance()->GetRoot()->PrintSelf();

	}
	if (KeyboardController::GetInstance()->IsKeyReleased('V'))
	{
		//*weaponRNode = DummyRANode->AddChild(weapon);
		//CSceneGraph ->GetRoot();
		/*	cout << CSceneGraph::GetInstance()->GetNumOfNode() << endl;
		cout<<CSceneGraph::GetInstance()->theRoot << endl;*/
		//CSceneGraph::GetInstance()->GetRoot()->PrintSelf();
		//CSceneGraph::GetInstance()->GetRoot()->DetachChild(weapon);
		
		GenericEntity *DummyHead = Create::Asset("Dummy_head", Vector3(0.f, 0.f, 0.f));
		CSceneNode *DummyHNode = CSceneGraph::GetInstance()->AddNode(DummyHead);
		//Math::RandFloatMinMax()
		CUpdateTransformation *mix = new CUpdateTransformation();
		mix->ApplyUpdate(0, 0.1f, 0);
		
		mix->SetSteps(0, 100);
		DummyHNode->SetUpdateTransformation(mix);
		
		//DummyHNode->ApplyTranslate(0.0f, 10.f, 0.f);
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
		//*weaponRNode = DummyRANode->AddChild(weapon);
		//CSceneGraph ->GetRoot();
		/*	cout << CSceneGraph::GetInstance()->GetNumOfNode() << endl;
		cout<<CSceneGraph::GetInstance()->theRoot << endl;*/
		//CSceneGraph::GetInstance()->GetRoot()->PrintSelf();
		//CSceneGraph::GetInstance()->GetRoot()->DetachChild(weapon);
		
		//CSceneNode *weaponRNode = CSceneGraph::GetInstance()->AddNode(weapon);
		//cout << CSceneGraph::GetInstance()<< endl;
		//CSceneGraph::GetInstance()->GetNode(Dummy_rightarm)->DeleteChild(weapon);
		//CSceneGraph::GetInstance()->GetNode(weapon)->Destroy();
		//CSceneGraph::GetInstance()->DeleteNode(*weapon);

		Vector3 dummypos = Vector3(entityposX, 10, entityposZ);
		//MeshBuilder::GetInstance()->GenerateCube("Dummy_head", Color(0, 0, 0), 1.f);
		//MeshBuilder::GetInstance()->GenerateCube("Dummy_body", Color(0, 1, 0), 1.f);
		//MeshBuilder::GetInstance()->GenerateCube("Dummy_arm", Color(0, 0, 1), 1.f);

		//TestDummy *Test = Create::Base("Dummy_head","Dummy_body","Dummy_arm","Dummy_arm", dummypos);

		TestDummy *Test = Create::dummy_part("Dummy_head",dummypos);
		TestDummy *Test1 = Create::dummy_part("Dummy_body",dummypos +Vector3(0,-1.1f,0));
		TestDummy *Test2 = Create::dummy_part("Dummy_arm", dummypos + Vector3(-1.1f, -1.1f, 0));
		TestDummy *Test3 = Create::dummy_part("Dummy_arm", dummypos + Vector3(1.1f, -1.1f, 0));

		
		CSceneNode *HeadNode = CSceneGraph::GetInstance()->AddNode(Test);
		CSceneNode *BodyNode = HeadNode->AddChild(Test1);
		CSceneNode *LANode = BodyNode->AddChild(Test2);
		CSceneNode *RANode = BodyNode->AddChild(Test3);

		//GenericEntity *weapon = Create::Asset("cone", Vector3(0.f, 0.f, 0.f));
		//CSceneNode *weaponRNode = DummyRANode->AddChild(weapon);

		/*GenericEntity *HeadDummycube = Create::Entity("cube", Vector3(entityposX, 1.1f, entityposZ));
		HeadDummycube->SetCollider(true);
		HeadDummycube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
		CSceneNode *HeadDummyNode = CSceneGraph::GetInstance()->AddNode(HeadDummycube);

		GenericEntity *BodyDummyCube = Create::Entity("cube", Vector3(entityposX, 0.0f, entityposZ));
		BodyDummyCube->SetCollider(true);
		BodyDummyCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
		CSceneNode* BodyDummyNode = HeadDummyNode->AddChild(BodyDummyCube);

		GenericEntity *RightArmDummyCube = Create::Entity("cube", Vector3(entityposX -1.1f, 0.0f, entityposZ));
		RightArmDummyCube->SetCollider(true);
		RightArmDummyCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
		CSceneNode *RightArmDummyNode = BodyDummyNode->AddChild(RightArmDummyCube);

		GenericEntity *LeftArmDummyCube = Create::Entity("cube", Vector3(entityposX+1.1f, 0.0f, entityposZ));
		LeftArmDummyCube->SetCollider(true);
		LeftArmDummyCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
		CSceneNode *LeftArmDummyNode = BodyDummyNode->AddChild(LeftArmDummyCube);
*/

	}
	countDown -= dt;
	if (countDown <= 0)
	{
		int entityposX = Math::RandIntMinMax(-40, 40);
		int entityposZ = Math::RandIntMinMax(-40, 40);

		Vector3 dummypos = Vector3(entityposX, 10, entityposZ);

		TestDummy *Test = Create::dummy_part("Dummy_head", dummypos);
		TestDummy *Test1 = Create::dummy_part("Dummy_body", dummypos + Vector3(0, -1.1f, 0));
		TestDummy *Test2 = Create::dummy_part("Dummy_arm", dummypos + Vector3(-1.1f, -1.1f, 0));
		TestDummy *Test3 = Create::dummy_part("Dummy_arm", dummypos + Vector3(1.1f, -1.1f, 0));


		CSceneNode *HeadNode = CSceneGraph::GetInstance()->AddNode(Test);
		CSceneNode *BodyNode = HeadNode->AddChild(Test1);
		CSceneNode *LANode = BodyNode->AddChild(Test2);
		CSceneNode *RANode = BodyNode->AddChild(Test3);
		countDown = 5.f;
	}
	// Update the player position and other details based on keyboard and mouse inputs
	playerInfo->Update(dt);

	//camera.Update(dt); // Can put the camera into an entity rather than here (Then we don't have to write this)

	GraphicsManager::GetInstance()->UpdateLights(dt);

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
	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
	playerInfo->DetachCamera();

	if (playerInfo->DropInstance() == false)
	{
#if _DEBUGMODE==1
		cout << "Unable to drop PlayerInfo class" << endl;
#endif
	}

	// Delete the lights
	delete lights[0];
	delete lights[1];
}
