class HandLoad : ItemLoad
{
    static override EntityAI Load(EntityAI oParent, PlayerBase oPlayer, ItemData oData)
    {
        if (!oPlayer || !oData) return NULL;
        int previousType = oData.LocationType;
        oData.LocationType = COM_GROUND;
        EntityAI entity = ItemLoad.Load(oParent, oPlayer, oData);
        oData.LocationType = previousType;
        if (entity) oPlayer.LocalTakeEntityToHands(entity);
        return entity;
    }
}