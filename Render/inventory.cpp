struct BLOCKDATA
{
    unsigned int ID;
    float3 Color;
};

BLOCKDATA BLOCKS[4] =
{
    { 0, (float3) { 0.0f, 0.0f, 0.0f } },
    { 1, (float3) { 150.0f, 75.0f, 0.0f } },
    { 2, (float3) { 0.0f, 255.0f, 0.0f } },
    { 3, (float3) { 127.0f, 127.0f, 127.0f } }
};


struct Inventory
{
    unsigned int Hotbar[9] = { 2, 1, 3, 0, 0, 0, 0, 0, 0};
    unsigned int selection = 0;

    BLOCKDATA GetBlockData(unsigned int hotbarIndex)
    {
        return BLOCKS[Hotbar[hotbarIndex]];
    }
};
