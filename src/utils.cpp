#include "utils.h"
#include <cmath>

Vector2 LerpVector2(Vector2 a, Vector2 b, float t)
{
    return Vector2{a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t};
}

Vector2 SmoothDamp(Vector2 current, Vector2 target, Vector2 *currentVelocity, float smoothTime, float maxSpeed, float deltaTime)
{

    smoothTime = std::fmaxf(0.0001f, smoothTime); //
    float omega = 2.0f / smoothTime;

    float x = omega * deltaTime;
    float exp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
    float change_x = current.x - target.x;
    float change_y = current.y - target.y;

    Vector2 originalTo = target;

    float maxChangeSq = maxSpeed * smoothTime;
    float sqDist = change_x * change_x + change_y * change_y;
    if (sqDist > maxChangeSq)
    {
        float mag = std::sqrt(sqDist);
        change_x = change_x / mag * maxChangeSq;
        change_y = change_y / mag * maxChangeSq;
    }
    target.x = current.x - change_x;
    target.y = current.y - change_y;

    float temp_x = (currentVelocity->x + omega * change_x) * deltaTime;
    float temp_y = (currentVelocity->y + omega * change_y) * deltaTime;

    currentVelocity->x = (currentVelocity->x - omega * temp_x) * exp;
    currentVelocity->y = (currentVelocity->y - omega * temp_y) * exp;

    float output_x = target.x + (change_x + temp_x) * exp;
    float output_y = target.y + (change_y + temp_y) * exp;

    float origMinusCurrent_x = originalTo.x - current.x;
    float origMinusCurrent_y = originalTo.y - current.y;
    float outMinusOrig_x = output_x - originalTo.x;
    float outMinusOrig_y = output_y - originalTo.y;

    if (origMinusCurrent_x * outMinusOrig_x + origMinusCurrent_y * outMinusOrig_y > 0)
    {
        output_x = originalTo.x;
        output_y = originalTo.y;
        currentVelocity->x = (output_x - originalTo.x) / deltaTime;
        currentVelocity->y = (output_y - originalTo.y) / deltaTime;
    }

    return Vector2{output_x, output_y};
}