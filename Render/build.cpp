#include <iostream>

RayCollision PlaceBlock(CUBE cubes[MAXCUBE], Player PLAYER)
{
    if(BlocksLength >= MAXCUBE)
    {
        return (RayCollision) { 0 };
    }
    
    Ray ray = {(Vector3) { PLAYER.pos.x, PLAYER.pos.y, PLAYER.pos.z}, normalized3((RVEC3(PLAYER.CAM.target)) - PLAYER.pos).V()};

    float NearestHit = 10.0f;
    RayCollision CurrentHit;

    for(int i = 0; i < BlocksLength; i ++)
    {
        BoundingBox bound = { (cubes[i].pos - (cubes[i].scale * 0.5f)).V(), (cubes[i].pos + (cubes[i].scale * 0.5f)).V() };

        RayCollision info = GetRayCollisionBox(ray, bound);

        if(info.distance <= NearestHit && info.hit)
        {
            NearestHit = info.distance;
            CurrentHit = info;
        }
    }

    return CurrentHit;

    // cubes[BlocksLength].scale = (float3) {2.0f, 2.0f, 2.0f};

    // cubes[BlocksLength].color = (float3) { 255.0f, 0.0f, 255.0f };
}

// TO COMPLETE: 
// FIX THE RAY CAST ISSUE