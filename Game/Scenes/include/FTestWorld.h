#pragma once
#include "FScene.h"
#include "FShader.h"
#include "FPlayer.h"
#include "FGrid.h"

class FTestWorld : public FScene
{
public:
    FTestWorld();
    ~FTestWorld();
    void Init();
    void BeforeLoad() override;
    void Render(float dt) override;
    void Logic(float dt) override;
    void Physics(float dt) override;

private:
    bool m_isActive;
    bool m_isLoaded;
    std::string m_name;
    int m_index;
    FShader shader;
    std::unique_ptr<FPlayer> player;
    std::unique_ptr<FGrid> grid;
    Texture2D red_sand_1;
    Texture2D red_sand_2;
};