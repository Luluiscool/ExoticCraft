#include <iostream>

#include "projection.cpp"
#include "world.cpp"
#include "build.cpp"
#include "inventory.cpp"

#include <rcamera.h>

Player PLAYER = { 0 };
bool Spectator = false;

RayCollision Bound;

Inventory inventory;


CUBE cubes[MAXCUBE];


void Setup(void)
{
    PLAYER.CAM = (Camera3D) { 0 };
    PLAYER.CAM.position = (Vector3){ 0.0f, 10.0f, 2.0f };  // Camera position
    // PLAYER.CAM.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    PLAYER.CAM.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    PLAYER.CAM.fovy = 80.0f;                                // Camera field-of-view Y
    PLAYER.CAM.projection = CAMERA_PERSPECTIVE;

    PLAYER.pos = (float3){0.0f, 9.25f, 2.0f};
    PLAYER.oldpos = (float3){0.0f, 9.25f, 2.f};
    PLAYER.scale = (float3){0.7f, 1.7f, 0.7f};

    DisableCursor(); 

    // World generation

    GenerateWorld(cubes);
    
}

void Loop(void)
{
    // Player movement

    CameraYaw(&PLAYER.CAM, -GetMouseDelta().x * 0.001f, false);
    CameraPitch(&PLAYER.CAM, -GetMouseDelta().y * 0.001f, true, false, false);

    PLAYER.NORMDIR = normalized3(RVEC3(PLAYER.CAM.target));

    Vector3 Forward = { 0.0f, 0.0f, 0.0f};
    Vector3 Right = { 0.0f, 0.0f, 0.0f};

    if(IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
    {
        Forward = (RVEC3(GetCameraForward(&PLAYER.CAM)) * 0.5f).V();
    }

    if(IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
    {
        Forward = (RVEC3(GetCameraForward(&PLAYER.CAM)) * -0.5f).V();
    }
    
    if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
    {
        Right = (RVEC3(GetCameraRight(&PLAYER.CAM)) * -0.5f).V();
    }

    if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
    {
        Right = (RVEC3(GetCameraRight(&PLAYER.CAM)) * 0.5f).V();
    }

    if(IsKeyDown(KEY_LEFT_SHIFT) && Spectator)
    {
        PLAYER.pos.y -= 0.2f;
        PLAYER.oldpos.y -= 0.2f;
        PLAYER.CAM.target.y -= 0.2f;
    }

    // For Spectator
    if(IsKeyPressed(KEY_Q))
    {
        Spectator = !Spectator;

        if(Spectator)
        {
            PLAYER.accelerate((float3) { 0.0f, 0.5f, 0.0f });
        }
    }



    int scrollDelta = (int) roundf(GetMouseWheelMove());
    
    if(inventory.selection == 0 && scrollDelta > 0)
    {
        inventory.selection = 8;
    }

    else
    {
        inventory.selection -= scrollDelta;

        if(inventory.selection > 8)
        {
            inventory.selection = 0;
        }
    }

    if(IsKeyPressed(49)) inventory.selection = 0;
    if(IsKeyPressed(50)) inventory.selection = 1;
    if(IsKeyPressed(51)) inventory.selection = 2;
    if(IsKeyPressed(52)) inventory.selection = 3;
    if(IsKeyPressed(53)) inventory.selection = 4;
    if(IsKeyPressed(54)) inventory.selection = 5;
    if(IsKeyPressed(55)) inventory.selection = 6;
    if(IsKeyPressed(56)) inventory.selection = 7;
    if(IsKeyPressed(57)) inventory.selection = 8;



    // Getting out of track and implementing physics

    PLAYER.OnGround = false;
    for(uint16_t i = 0; i < BlocksLength; i ++)
    {
        PLAYER.solveCollision(cubes[i], Spectator);
    }
    

    Vector3 Direction = ((normalized3(RVEC3({ Forward.x + Right.x, 1.0f, Forward.z + Right.z }))) * PLAYER.SPEED).V();

    PLAYER.pos = (float3) { PLAYER.pos.x + Direction.x, PLAYER.pos.y, PLAYER.pos.z + Direction.z };

    // Back to controls

    if(IsKeyDown(KEY_SPACE))
    {
        if(Spectator)
        {
            PLAYER.pos.y += 0.2f;
            PLAYER.oldpos.y += 0.2f;
            PLAYER.CAM.target.y += 0.2f;
        }
        else if(PLAYER.OnGround)
        {
            PLAYER.accelerate((float3) { 0.0f, 0.75f, 0.0f });
        }
    }

    PLAYER.CAM.target = (Vector3) { PLAYER.CAM.target.x + Direction.x, PLAYER.CAM.target.y, PLAYER.CAM.target.z + Direction.z };

    Bound = PlaceBlock(cubes, PLAYER);

    PLAYER.CAM.position = (PLAYER.pos).V();

    PLAYER.NEXT(1, !Spectator);

}

void Renderloop(void)
{
    for(uint16_t i = 0; i < BlocksLength; i ++)
    {
        cubes[i].RenderProjection();
    }

    float3 FocusPoint = (float3) { floorf(Bound.point.x), floorf(Bound.point.y), floorf(Bound.point.z) };

    BoundingBox bound = {
        (Vector3) { FocusPoint.x - 0.505f, FocusPoint.y - 0.505f, FocusPoint.z - 0.505f},
        (Vector3) { FocusPoint.x + 0.505f, FocusPoint.y + 0.505f, FocusPoint.z + 0.505f}
    };

    FocusPoint += RVEC3(Bound.normal);

    if(Bound.hit)
    {
        DrawBoundingBox(bound, WHITE);

        if(IsMouseButtonPressed(1) && BlocksLength < MAXCUBE - 1 && !CheckCollisionBoxes(PLAYER.GetBoundingBox(), (BoundingBox) { (FocusPoint - 0.5f).V(), (FocusPoint + 0.5f).V() }))
        {
            cubes[BlocksLength].pos = FocusPoint;

            BLOCKDATA BlockPlacementData = inventory.GetBlockData(inventory.selection);

            if(BlockPlacementData.ID != 0)
            {
                cubes[BlocksLength].color = BlockPlacementData.Color;

                BlocksLength ++;
            }
        }
        if(IsMouseButtonPressed(0))
        {
            for(int i = 0; i < BlocksLength; i ++)
            {
                if(cubes[i].pos.x == floor(Bound.point.x) && cubes[i].pos.y == floor(Bound.point.y) && cubes[i].pos.z == floor(Bound.point.z))
                {
                    // Swapping
                    CUBE A = cubes[BlocksLength - 1];

                    cubes[BlocksLength - 1] = cubes[i];
                    cubes[i] = A;

                    BlocksLength --;
                    break;
                }
            }
        }
    }

    // PLAYER.RenderProjection();

    DrawGrid(100, 1.0f);
}
