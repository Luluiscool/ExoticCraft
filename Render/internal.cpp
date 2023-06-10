#include <iostream>
#include "projection.cpp"
#include <rcamera.h>
#include "world.cpp"
#include "build.cpp"

Player PLAYER = { 0 };
bool Spectator = false;

RayCollision Bound;


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

    // PLAYER.ANGLEX = GetMouseDelta().x * 0.003f;
    // PLAYER.ANGLEY = GetMouseDelta().y * 0.003f;
    

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

    if(IsKeyDown(KEY_E) && Spectator)
    {
        PLAYER.pos.y += 0.2;
        PLAYER.oldpos.y += 0.2;
        PLAYER.CAM.target.y += 0.2;
    }

    if(IsKeyDown(KEY_LEFT_SHIFT) && Spectator)
    {
        PLAYER.pos.y -= 0.2;
        PLAYER.oldpos.y -= 0.2;
        PLAYER.CAM.target.y -= 0.2;
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

    PLAYER.OnGround = false;
    for(uint16_t i = 0; i < MAXCUBE; i ++)
    {
        PLAYER.solveCollision(cubes[i], Spectator);
    }
    

    Vector3 Direction = ((normalized3(RVEC3({ Forward.x + Right.x, 1.0f, Forward.z + Right.z }))) * PLAYER.SPEED).V();
    // if(RVEC3(Direction).length() > 0.0f)
    // {
        PLAYER.pos = (float3) { PLAYER.pos.x + Direction.x, PLAYER.pos.y, PLAYER.pos.z + Direction.z };

        if(PLAYER.OnGround && IsKeyDown(KEY_SPACE))
        {
            PLAYER.accelerate((float3) { 0.0f, 0.75f, 0.0f });
        }
        else
        {
            // PLAYER.oldpos = PLAYER.pos;
        }

        PLAYER.CAM.target = (Vector3) { PLAYER.CAM.target.x + Direction.x, PLAYER.CAM.target.y, PLAYER.CAM.target.z + Direction.z };
    // }

    // if(IsKeyDown(KEY_LEFT_SHIFT))
    // {
    //     PLAYER.CAM.position.y -= 0.1f;
    //     PLAYER.CAM.target.y -= 0.1f;
    // }
    
    // if(IsKeyDown(KEY_E))
    // {
    //     PLAYER.CAM.position.y += 0.1f;
    //     PLAYER.CAM.target.y += 0.1f;
    // }

    // if(GetKeyPressed() == KEY_Q)
    // {
    //     Spectator = !Spectator;
    // }

    // PLAYER.pos = RVEC3(PLAYER.CAM.position);

    // if(GetKeyPressed() == KEY_SPACE)
    // {
        
    // }

    // PLAYER.NORMDIR = (float3) { cosf(PLAYER.ANGLEX), sinf(PLAYER.ANGLEX),  cosf(PLAYER.ANGLEY)};

    Bound = PlaceBlock(cubes, PLAYER);

    PLAYER.CAM.position = (PLAYER.pos).V();

    PLAYER.NEXT(1, !Spectator);

}

void Renderloop(void)
{
    for(uint16_t i = 0; i < MAXCUBE; i ++)
    {
        cubes[i].RenderProjection();
    }

    float3 FocusPoint = (float3) { floorf(Bound.point.x), floorf(Bound.point.y), floorf(Bound.point.z) };

    BoundingBox bound = {
        (Vector3) { FocusPoint.x - 0.52f, FocusPoint.y - 0.52f, FocusPoint.z - 0.52f},
        (Vector3) { FocusPoint.x + 0.52f, FocusPoint.y + 0.52f, FocusPoint.z + 0.52f}
    };

    FocusPoint += RVEC3(Bound.normal);

    if(Bound.hit)
    {
        DrawBoundingBox(bound, WHITE);

        if(IsMouseButtonPressed(1))
        {
            cubes[BlocksLength].pos = FocusPoint;
            cubes[BlocksLength].color = (float3) { 127.0f, 127.0f, 127.0f };
            BlocksLength ++;
        }
        if(IsMouseButtonPressed(0))
        {
            for(int i = 0; i < BlocksLength; i ++)
            {
                if(cubes[i].pos.x == floor(Bound.point.x) && cubes[i].pos.y == floor(Bound.point.y) && cubes[i].pos.z == floor(Bound.point.z))
                {
                    std :: cout << "Breaking?" << std :: endl;
                    // Swapping
                    CUBE A = cubes[BlocksLength];

                    cubes[BlocksLength] = cubes[i];
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
