class COMOfflineAmmo
{
    protected static bool s_Enabled;
    static void SetEnabled(bool enabled) { s_Enabled = enabled; }
    static bool IsEnabled() { return s_Enabled; }
    static void Refill(Weapon_Base weapon, int muzzle)
    {
        if (!s_Enabled || !GetGame() || GetGame().IsMultiplayer() || !weapon) return;
        PlayerBase owner = PlayerBase.Cast(weapon.GetHierarchyRootPlayer());
        if (!owner || owner != GetGame().GetPlayer() || !owner.IsAlive()) return;
        Magazine magazine = weapon.GetMagazine(muzzle);
        if (magazine && !magazine.IsDamageDestroyed() && magazine.GetAmmoCount() < magazine.GetAmmoMax())
            magazine.ServerSetAmmoCount(magazine.GetAmmoMax());
        if (weapon.HasInternalMagazine(muzzle) && !weapon.IsInternalMagazineFull(muzzle))
            weapon.FillInnerMagazine("", WeaponWithAmmoFlags.NONE);
        // Preserve health, attachments, overheating, jams and chamber state.
    }
}
modded class Weapon_Base
{
    override void EEFired(int muzzleType, int mode, string ammoType)
    {
        super.EEFired(muzzleType, mode, ammoType);
        COMOfflineAmmo.Refill(this, muzzleType);
    }
}
// Native EEFired runs before automatic cycling takes the next cartridge.
// Refill after that feed as well, so the magazine remains full.
modded class WeaponFireAndChamber
{
    override void OnEntry(WeaponEventBase e)
    {
        super.OnEntry(e);
        COMOfflineAmmo.Refill(m_weapon, m_weapon.GetCurrentMuzzle());
    }
}
modded class WeaponFireAndChamberFromInnerMagazine
{
    override void OnEntry(WeaponEventBase e)
    {
        super.OnEntry(e);
        COMOfflineAmmo.Refill(m_weapon, m_weapon.GetCurrentMuzzle());
    }
}