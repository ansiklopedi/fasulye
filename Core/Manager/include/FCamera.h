#pragma once
#include "raylib.h"
#include "enums.h"
#include "FStruct.h"
#include <vector>

class FCamera
{
private:
    static Camera2D m_camera;
    std::vector<CameraEffect> m_effects;

public:
    FCamera();
    FCamera(Vector2 target);

    void SetZoom(float zoom);
    void SetTarget(Vector2 target);
    void SetRotation(float rotation);

    FCameraViewport GetCameraViewport();
    Rectangle GetCameraViewportAsRectangle();
    void AddEffect(CameraEffect effect);
    void RemoveEffect(CameraEffect effect);
    static Camera2D GetCamera() { return m_camera; }
};
