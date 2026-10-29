class ItemLoad
{
    static EntityAI Load(EntityAI oParent, PlayerBase oPlayer, ItemData oData) {
        if (!oParent || !oPlayer || !oData || oData.Name == "") return NULL;
        EntityAI oEntity;

        switch(oData.LocationType)
        {
            case COM_CARGO_TYPE:
                oEntity = oParent.GetInventory().CreateEntityInCargoEx(oData.Name, oData.CargoIndex, oData.X, oData.Y, oData.Flipped);
                break;
            case COM_ATTACHMENT_TYPE:
                if (oData.Version >= 2 && oData.AttachmentSlot >= 0)
                    oEntity = oParent.GetInventory().CreateAttachmentEx(oData.Name, oData.AttachmentSlot);
                else
                    oEntity = oParent.GetInventory().CreateAttachment(oData.Name);
                break;
            case COM_GROUND:
                oEntity = EntityAI.Cast(GetGame().CreateObjectEx(oData.Name, oPlayer.GetPosition(), ECE_CREATEPHYSICS | ECE_KEEPHEIGHT));
                break;
            case COM_DEFAULT_TYPE:
            default:
                oEntity = oParent.GetInventory().CreateInInventory(oData.Name);
                break;
        }

        if (!oEntity)
        {
            Print("[COM] Cannot restore inventory item: " + oData.Name);
            return NULL;
        }
        ItemBase oItem = NULL;
        
        if (Class.CastTo(oItem, oEntity) && oEntity.IsItemBase())
        {
            oItem.SetWet(oData.ItemWet);
            oItem.SetHealth("", "", oData.ItemHealth);

            if (oItem.IsInherited(Magazine))
            {
                Magazine oMagazine = Magazine.Cast( oItem );
                oMagazine.ServerSetAmmoCount(oData.Quantity);
            } else
            {
                oItem.SetQuantity(oData.Quantity);
            }

            if (oData.Version >= 2)
            {
                if (oItem.HasEnergyManager()) oItem.GetCompEM().SetEnergy(oData.Energy);
                oItem.SetLiquidType(oData.LiquidType);
                oItem.SetTemperatureDirect(oData.Temperature);
                if (oItem.GetFoodStage() && oData.SavedFoodStage >= 0) oItem.GetFoodStage().ChangeFoodStage(oData.SavedFoodStage);
            }
            if (oData.IQuickBarIndex >= 0 && oData.IQuickBarIndex < 10) 
            {
                oPlayer.SetQuickBarEntityShortcut(oItem, oData.IQuickBarIndex);
            }
        }

        if (oData.NumAttachments > 0)
        {
            for (int iAttachment = 0; iAttachment < oData.ItemAttachments.Count(); iAttachment++) 
            {
                //ItemLoad.Load(oEntity, oPlayer, oData.ItemAttachments[iAttachment]);
                FixAndLoadAttachment(oEntity, oPlayer, oData.ItemAttachments[iAttachment]);
            }
        }

        if (oData.NumItems > 0)
        {
            for (int iItem = 0; iItem < oData.Items.Count(); iItem++) 
            {
                ItemLoad.Load(oEntity, oPlayer, oData.Items[iItem]);
            }
        }

        Weapon_Base weapon = Weapon_Base.Cast(oEntity);
        if (weapon && oData.Version >= 2)
        {
            foreach (COMCartridgeData round : oData.Cartridges)
            {
                if (!round || round.SavedMuzzle < 0 || round.SavedMuzzle >= weapon.GetMuzzleCount() || round.SavedBulletType == "") continue;
                if (round.InChamber) weapon.PushCartridgeToChamber(round.SavedMuzzle, round.Damage, round.SavedBulletType);
                else weapon.PushCartridgeToInternalMagazine(round.SavedMuzzle, round.Damage, round.SavedBulletType);
            }
            if (oData.CurrentMuzzle >= 0 && oData.CurrentMuzzle < weapon.GetMuzzleCount()) weapon.SetCurrentMuzzle(oData.CurrentMuzzle);
            weapon.ValidateAndRepair();
            weapon.Synchronize();
        }
        return oEntity;
    }

    protected static EntityAI FixAndLoadAttachment(EntityAI parent, PlayerBase player, ItemData data)
    {
        return ItemLoad.Load(parent, player, data);
    }
}