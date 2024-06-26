#include "FScene.h"
#include "raylib.h"
#include "FGame.h"
#include "FSceneManager.h"
#include <iostream>
#include "FTestWorld.h"
#include "FMainMenu.h"

FGame::FGame(int width, int height)
{
    InitWindow(width, height, "FSun");
    SetTargetFPS(240);

    m_height = height;
    m_width = width;

    std::unique_ptr<FScene> mainMenu = std::unique_ptr<FScene>(new FMainMenu());
    FSceneManager::AddScene(std::move(mainMenu));

    // SceneMenager = FSceneManager();
    std::unique_ptr<FScene> scene = std::unique_ptr<FScene>(new FTestWorld());
    FSceneManager::AddScene(std::move(scene));

    FSceneManager::SetActiveScene(0);
}

FGame::~FGame()
{
    CloseWindow();
}

void DrawGridChat(Camera2D camera, int gridSize)
{
    // Calculate the starting point of the grid based on the camera offset
    int startX = (int)(camera.target.x - (GetScreenWidth() / 2.0f));
    int startY = (int)(camera.target.y - (GetScreenHeight() / 2.0f));

    // Draw vertical lines
    for (int x = startX - (startX % gridSize); x < camera.target.x + (GetScreenWidth() / 2.0f); x += gridSize)
    {
        DrawLine(x, -GetScreenHeight(), x, GetScreenHeight() * 2, LIGHTGRAY);
    }

    // Draw horizontal lines
    for (int y = startY - (startY % gridSize); y < camera.target.y + (GetScreenHeight() / 2.0f); y += gridSize)
    {
        DrawLine(-GetScreenWidth(), y, GetScreenWidth() * 2, y, LIGHTGRAY);
    }
}

void FGame::Run()
{
    std::cout << "FGame::Run()" << std::endl;
    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        // DrawText("Fasulye", GetScreenWidth() / 2, GetScreenHeight() / 2, 20, WHITE);
        std::unique_ptr<FScene> &activeScene = FSceneManager::GetActiveScene();
        // Physics update loop
        activeScene->Physics(dt);
        // Game logic update loop
        activeScene->Logic(dt);
        // Drawing loop
        ClearBackground(BLACK);
        BeginDrawing();
        BeginMode2D(activeScene->camera.GetCamera());

        DrawGridChat(activeScene->camera.GetCamera(), 32);
        activeScene->Render(dt);

        EndMode2D();

        DrawFPS(GetScreenWidth() / 6, GetScreenHeight() / 4);

        EndDrawing();

        if (IsKeyPressed(KEY_F))
        {
            // see what display we are on right now
            int display = GetCurrentMonitor();

            if (IsWindowFullscreen())
            {
                // if we are full screen, then go back to the windowed size
                SetWindowSize(m_width, m_height);
            }
            else
            {
                // if we are not full screen, set the window size to match the monitor we are on
                SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
            }

            // toggle the state
            ToggleFullscreen();
        }
    }
}