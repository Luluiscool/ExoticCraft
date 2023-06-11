RayCollision PlaceBlock(CUBE cubes[MAXCUBE], Player PLAYER)
{
    if(BlocksLength >= MAXCUBE)
    {
        return (RayCollision) { 0 };
    }
    
    Ray ray = {(Vector3) { PLAYER.CAM.position.x, PLAYER.CAM.position.y, PLAYER.CAM.position.z}, normalized3((RVEC3(PLAYER.CAM.target)) - RVEC3(PLAYER.CAM.position)).V()};

    float NearestHit = 50.0f;
    RayCollision CurrentHit;
    float3 CurrentCubeOrigin = { 0 };

    for(int i = 0; i < BlocksLength; i ++)
    {
        BoundingBox bound = { (cubes[i].pos - (cubes[i].scale * 0.5f)).V(), (cubes[i].pos + (cubes[i].scale * 0.5f)).V() };

        RayCollision info = GetRayCollisionBox(ray, bound);

        if(info.hit && info.distance < NearestHit)
        {
            NearestHit = info.distance;
            CurrentHit = info;
            CurrentCubeOrigin = cubes[i].pos;
        }
    }

    CurrentHit.point = CurrentCubeOrigin.V();

    return CurrentHit;
}
