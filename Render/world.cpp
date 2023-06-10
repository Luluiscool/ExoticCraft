void GenerateWorld(CUBE cubes[MAXCUBE])
{
    const int MAXX = 31;
    const int MAXY = 31;

    BlocksLength = MAXX * MAXY;

    int cb = 0;

    for(int i = 0; i < MAXX; i ++)
    {
        for(int j = 0; j < MAXY; j ++)
        {
            cubes[cb].pos.x = i;
            cubes[cb].pos.z = j;
            cubes[cb].pos.y = 0.0f;
            if(RNG() < 0.2f)
            {
                cubes[cb].color = (float3) { 150.0f, 75.0f, 0.0f } + (float)GetRandomValue(0, 30);
            }
            else
            {
                cubes[cb].color = (float3) { 0.0f + (float)GetRandomValue(0, 30), 255.0f - (float)GetRandomValue(0, 30), 0.0f + (float)GetRandomValue(0, 30)};
            }


            cb ++;
        }
    }
}
