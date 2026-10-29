class InventoryLoad : ItemLoad
{
    static override EntityAI Load(EntityAI oParent, PlayerBase oPlayer, ItemData oData)
    {
        if (!oParent || !oPlayer || !oData) return NULL;
        foreach (ItemData attachment : oData.ItemAttachments)
            if (attachment) FixAndLoadAttachment(oParent, oPlayer, attachment);
        foreach (ItemData item : oData.Items)
            if (item) ItemLoad.Load(oParent, oPlayer, item);
        return oParent;
    }
}