#include <iostream>
#include "Render/internal.cpp"

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

#define MAX_POSTPRO_SHADERS         12

const float2 Center = resolution / 2;

float FrameTime = 0.0f;
int Tick = 0.0f;
Shader shader;

int main () {
    InitWindow(resolution.x, resolution.y, "ExoticCraft v0.0.2 Indev");

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetTargetFPS(60);

    Setup();

    while (WindowShouldClose() == false){
        FrameTime = GetFrameTime();

        Loop();

        BeginDrawing();
            ClearBackground(BLACK);

            if(IsKeyDown(KEY_LEFT_SHIFT))
            {
                PLAYER.CAM.position.y -= 0.2f;
                BeginMode3D(PLAYER.CAM);

                    Renderloop();

                EndMode3D();
                PLAYER.CAM.position.y += 0.2f;
            }
            else
            {
                BeginMode3D(PLAYER.CAM);

                    Renderloop();

                EndMode3D();
            }

            DrawText((std :: string("DeltaTime : ") + std :: to_string(FrameTime * 1000)).c_str(), 20, 20, 20, BLUE);
            DrawText((std :: string("FPS : ") + std :: to_string(int(1 / FrameTime))).c_str(), 20, 50, 20, GREEN);
            DrawText((std :: string("CAMERA ANGLE = ") + std :: to_string(PLAYER.NORMDIR.x) + std :: string(" ") + std :: to_string(PLAYER.NORMDIR.y) + std :: string(" ") + std :: to_string(PLAYER.NORMDIR.z)).c_str(), 20, 80, 20, ORANGE);
            DrawText((std :: string("CAMERA Position = ") + std::to_string(PLAYER.CAM.position.x) + std :: string(" ") + std::to_string(PLAYER.CAM.position.y) + std :: string(" ") + std :: to_string(PLAYER.CAM.position.z)).c_str(), 20, 110, 20, RED);
            DrawText((std :: string("PLAYER Position = ") + std::to_string(PLAYER.pos.x) + std :: string(" ") + std::to_string(PLAYER.pos.y) + std :: string(" ") + std :: to_string(PLAYER.pos.z)).c_str(), 20, 140, 20, YELLOW);
            DrawText((std :: string("OnGround = ") + std::to_string(PLAYER.OnGround)).c_str(), 20, 170, 20, RAYWHITE);
            DrawText((std :: string("SpectatorMode = ") + std::to_string(Spectator)).c_str(), 20, 200, 20, LIME);
            DrawText((std :: string("Blocks = ") + std::to_string(BlocksLength)).c_str(), 20, 230, 20, SKYBLUE);

            // Drawing the crosshair

            DrawText("+", center.x, center.y, 30, WHITE);

            // Drawing Inventory

            float2 hotbarLeftPosition = (float2) { center.x - (4.5f * 50.0f), resolution.y - 50.0f };
            for(int i = 0; i < 9; i ++)
            {
                float2 hotbarBoxPosition = hotbarLeftPosition + (float2) {((float)i * 50.0f), 0.0f};

                DrawRectangle(hotbarBoxPosition.x, hotbarBoxPosition.y, 50, 50, ToRenderColor(inventory.GetBlockData(i).Color));

                if(inventory.selection != i)
                {
                    DrawRectangleLines(hotbarBoxPosition.x, hotbarBoxPosition.y, 50, 50, (Color) { LIGHTGRAY.r, LIGHTGRAY.g, LIGHTGRAY.b, 127});
                }
                
                else
                {
                    DrawRectangleLines(hotbarBoxPosition.x, hotbarBoxPosition.y, 50, 50, WHITE);
                }
                
            }

        EndDrawing();
    }

    CloseWindow();

    system("cls");
    return 0;
}
