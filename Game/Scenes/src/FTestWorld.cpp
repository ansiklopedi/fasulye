#include "FTestWorld.h"
#include "FItem.h"
#include "FSceneManager.h"
#include <iostream>
#include "FUtils.h"
#include "enums.h"

FTestWorld::FTestWorld()
{
	std::cout << "FTestWorld constructor" << std::endl;
}

void FTestWorld::Init()
{
	// Works after loading but only called when setActiveScene called
	player = std::unique_ptr<FPlayer>(new FPlayer({static_cast<float>(GetScreenWidth() / 2), static_cast<float>(GetScreenHeight() / 2)}));
	camera.AddEffect(CameraEffect::NoneEffect);
	camera.SetZoom(2.0f);

	shader.FLoadShader("sandColor.fs", ShaderType::Fragment);
	int tintColorLoc = GetShaderLocation(shader.m_shaders[0], "u_tintColor");
	Vector3 tintColor = {1.0, 0.0, 0.0};
	shader.FSetValue(0, tintColorLoc, &tintColor, SHADER_UNIFORM_VEC3);

	red_sand_1 = LoadTexture("resources/assets/tiles/gray_sand_1.png");
	red_sand_2 = LoadTexture("resources/assets/tiles/gray_sand_2.png");

	std::cout << "FTestWorld Init" << std::endl;

	std::unique_ptr<FItem> item = std::unique_ptr<FItem>(new FItem({static_cast<float>(GetScreenWidth() / 2) + 64, static_cast<float>(GetScreenHeight() / 2)}, LoadTexture("resources/assets/chest.png")));
	FSceneManager::AddObjectToActiveScene(std::move(item));

	std::cout << "FTestWorld Init" << std::endl;

	std::unique_ptr<FItem> item2 = std::unique_ptr<FItem>(new FItem({static_cast<float>(GetScreenWidth() / 2) + 32, static_cast<float>(GetScreenHeight() / 2) + 32}, LoadTexture("resources/assets/chest.png")));
	FSceneManager::AddObjectToActiveScene(std::move(item2));
}

void FTestWorld::BeforeLoad()
{
	std::cout << "FTestWorld BeforeLoad" << std::endl;
	grid = std::unique_ptr<FGrid>(new FGrid(100, 100));

	for (int i = 0; i < grid->GetWidth(); i++)
	{
		for (int j = 0; j < grid->GetHeight(); j++)
		{
			if (RandomRange(0, 10) < 5)
				grid->SetTile(i, j, 0);
			else
				grid->SetTile(i, j, 1);
		}
	}
	// std::cout << grid->GetWidth() << std::endl;
}

FTestWorld::~FTestWorld()
{
	std::cout << "FTestWorld destructor" << std::endl;
	UnloadTexture(red_sand_1);
	UnloadTexture(red_sand_2);
}

void FTestWorld::Render(float dt)
{
	Vector2 vec = GetScreenToWorld2D(GetMousePosition(), camera.GetCamera());
	camera.SetTarget({player->position.x + 48, player->position.y + 40});

	Rectangle cameraBox = camera.GetCameraViewportAsRectangle();
	DrawRectangle(cameraBox.x - 64, cameraBox.y - 64, cameraBox.width + 64, cameraBox.height + 64, BLUE);
	BeginShaderMode(shader.m_shaders[0]);

	for (int x = RoundAccordingToRef(cameraBox.x, 32); x < grid->GetWidth() * 32; x += 32)
	{
		for (int y = RoundAccordingToRef(cameraBox.y, 32); y < grid->GetHeight() * 32; y += 32)
		{
			if (CheckCollisionRecs({(float)x, (float)y, 32, 32}, cameraBox))
			{
				if (grid->GetTileWithWorldPosition({(float)x, (float)y}) == 1)
				{
					DrawTexture(red_sand_1, x, y, WHITE);
				}
				else
				{
					DrawTexture(red_sand_2, x, y, WHITE);
				}
			}

			if (y > (cameraBox.y + cameraBox.height + 64))
				break;
		}

		if (x > (cameraBox.x + cameraBox.width + 64))
			break;
	}
	EndShaderMode();

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->SetMousePosition(vec);
		objects[i]->Draw(dt);
		objects[i]->ShowBoundingBox();
	}

	this->player->SetMousePosition(vec);
	this->player->Draw(dt);
	this->player->ShowBoundingBox();
}

void FTestWorld::Logic(float dt)
{
	if (IsKeyPressed(KEY_C))
	{
		int tintColorLoc = GetShaderLocation(shader.m_shaders[0], "u_tintColor");
		float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		float g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		float b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		Vector3 tintColor = {r, g, b};
		shader.FSetValue(0, tintColorLoc, &tintColor, SHADER_UNIFORM_VEC3);
	}

	std::vector<int> destroyedObjects;
	player->Update(dt);
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->isDestroyed)
		{
			destroyedObjects.push_back(i);
		}
		else
		{
			objects[i]->Update(dt);
		}
	}
	for (size_t i = 0; i < destroyedObjects.size(); i++)
	{
		objects[destroyedObjects[i]].reset();
		objects.erase(objects.begin() + destroyedObjects[i]);
	}
	destroyedObjects.clear();
}

void FTestWorld::Physics(float dt)
{
	for (int i = 0; i < objects.size() - 1; i++)
	{
		for (int j = 1; j < objects.size(); j++)
		{
			if (i != j)
			{
				if (CheckCollisionRecs(objects[i]->GetBoundingBox(), objects[j]->GetBoundingBox()))
				{
					objects[i]->Physics(objects[j]->GetTag(), objects[j]->GetBoundingBox());
					objects[j]->Physics(objects[i]->GetTag(), objects[i]->GetBoundingBox());
				}
			}
		}
	}

	for (int i = 0; i < objects.size(); i++)
	{
		if (CheckCollisionRecs(objects[i]->GetBoundingBox(), player->GetBoundingBox()))
		{
			objects[i]->Physics(player->GetTag(), player->GetBoundingBox());
			player->Physics(objects[i]->GetTag(), objects[i]->GetBoundingBox());
		}
	}
}
