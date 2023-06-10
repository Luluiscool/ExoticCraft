#include <iostream>
#include <raylib.h>
#include "../vector.cpp"
#define MAXCUBE 1000

int BlocksLength = 0;


// Convert 3D space coordinates to 2D projection coordinates
float2 GetProjection2D(float3 pos)
{
    // Converting to 2D space coordinates
    float2 pointProjection = (float2) { pos.x / pos.z, pos.y / pos.z };

    // Return the translated vector
    return pointProjection + center;
}

struct CUBE
{
    float3 pos;
    float3 scale = (float3) {1, 1, 1};
    float3 color = (float3) {255, 255, 255};

    // Render the projection
    void RenderProjection(void)
    {
        // BoundingBox bound = { (pos - (scale * 0.5f)).V(), (pos + (scale * 0.5f)).V() };
        DrawCubeV(pos.V(), scale.V(), (Color) {(unsigned char)color.x, (unsigned char)color.y, (unsigned char)color.z, 255});
        // DrawBoundingBox(bound, WHITE);
    }
};

const float3 Gravity = (float3) { 0.0f, -0.05f, 0.0f };
const float3 Friction = (float3) { 0.8f, 0.8f, 0.8f };

struct Player
{
    float3 pos;
    float3 oldpos;
    float3 acceleration;
    Camera3D CAM;
    float3 NORMDIR;
    float ANGLEX;
    float ANGLEY;
    bool OnGround = false;
    float SPEED = 0.1;

    float3 scale = (float3) {1, 1, 1};

    // Render the projection
    void RenderProjection(void)
    {
        DrawCubeV(pos.V(), scale.V(), WHITE);
    }

    void update(float dt, bool IsSpectator)
    {
        const float3 velocity = pos - oldpos;

        oldpos = pos;

        // Xn + 1 = Xn + v + aΔt2
        pos = pos + (velocity * (IsSpectator ? Friction + 0.1f : Friction)) + (acceleration * dt * dt);

        CAM.target = (Vector3) { CAM.target.x + (pos.x - oldpos.x), CAM.target.y + (pos.y - oldpos.y), CAM.target.z + (pos.z - oldpos.z) };

        acceleration = (float3) { 0.0f, 0.0f, 0.0f };
    }

    void accelerate(float3 acc)
    {
        acceleration += acc;
    }

    void NEXT(float dt, bool AccelerateDueToGravity)
    {
        if(AccelerateDueToGravity)
        {
            accelerate(Gravity);
        }

        update(dt, !AccelerateDueToGravity);
    }

    void solveCollision(CUBE block, bool IsSpectator)
    {
        if(IsSpectator) return;
        BoundingBox a;
        a.min = (block.pos - (block.scale * 0.5)).V();
        a.max = (block.pos + (block.scale * 0.5)).V();

        BoundingBox b;
        b.min = (pos - (scale * 0.5)).V();
        b.max = (pos + (scale * 0.5)).V();

        if(CheckCollisionBoxes(a, b))
        {
            Rectangle A = { a.min.x, a.min.y, block.scale.x, block.scale.y };
            Rectangle B = { b.min.x, b.min.y, scale.x, scale.y };

            Rectangle C = GetCollisionRec(A, B);


            pos.y += C.height;

            if(!OnGround)
            CAM.target.y -= Gravity.y;
            
            OnGround = true;
        }
    }
};
