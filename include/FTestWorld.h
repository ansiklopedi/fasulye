#pragma once
#include "FScene.h"

class FTestWorld : public FScene
{
public:
    FTestWorld();
    ~FTestWorld();
    void Init();
    void BeforeLoad() override;

private:
    bool m_isActive;
    bool m_isLoaded;
    std::string m_name;
    int m_index;
};