class COMGameplayToggles extends Module
{
    static const int STAMINA_MODIFIER = 173742;
    protected PlayerBase m_Player;
    protected bool m_DamageApplied;
    protected bool m_PreviousDamage;
    protected bool m_StaminaApplied;
    protected bool m_UnlimitedAmmo;
    protected bool m_InfiniteStamina;
    void ~COMGameplayToggles() { ReleasePlayer(); COMOfflineAmmo.SetEnabled(false); }
    override void onMissionFinish() { ReleasePlayer(); COMOfflineAmmo.SetEnabled(false); }
    void ReleasePlayer()
    {
        if (GetGame() && m_Player)
        {
            if (m_DamageApplied) m_Player.SetAllowDamage(m_PreviousDamage);
            StaminaHandler stamina = m_Player.GetStaminaHandler();
            if (m_StaminaApplied && stamina)
            {
                stamina.DeactivateDepletionModifier(STAMINA_MODIFIER);
                stamina.m_RegisteredDepletionModifiers.Remove(STAMINA_MODIFIER);
            }
        }
        m_Player = NULL;
        m_DamageApplied = false;
        m_StaminaApplied = false;
    }
    bool GetGodMode() { return m_COM_GodMode; }
    bool GetInfiniteStamina() { return m_InfiniteStamina; }
    bool GetUnlimitedAmmo() { return m_UnlimitedAmmo; }
    void SetGodMode(bool enabled) { m_COM_GodMode = enabled; onUpdate(0); }
    void SetInfiniteStamina(bool enabled) { m_InfiniteStamina = enabled; onUpdate(0); }
    void SetUnlimitedAmmo(bool enabled) { m_UnlimitedAmmo = enabled; onUpdate(0); }
    override void onUpdate(float timeslice)
    {
        if (!GetGame() || GetGame().IsMultiplayer()) return;
        PlayerBase current = COM_GetPB();
        if (current != m_Player) { ReleasePlayer(); m_Player = current; }
        COMOfflineAmmo.SetEnabled(m_UnlimitedAmmo);
        if (!m_Player || !m_Player.IsAlive()) return;
        if (m_COM_GodMode && !m_DamageApplied)
        {
            m_PreviousDamage = m_Player.GetAllowDamage();
            m_Player.SetAllowDamage(false);
            m_DamageApplied = true;
        }
        else if (!m_COM_GodMode && m_DamageApplied)
        {
            m_Player.SetAllowDamage(m_PreviousDamage);
            m_DamageApplied = false;
        }
        StaminaHandler stamina = m_Player.GetStaminaHandler();
        if (stamina && m_InfiniteStamina)
        {
            if (!m_StaminaApplied)
            {
                stamina.m_RegisteredDepletionModifiers.Insert(STAMINA_MODIFIER, 0);
                stamina.ActivateDepletionModifier(STAMINA_MODIFIER);
                m_StaminaApplied = true;
            }
            stamina.SetStamina(stamina.GetStaminaMax());
        }
        else if (stamina && m_StaminaApplied)
        {
            stamina.DeactivateDepletionModifier(STAMINA_MODIFIER);
            stamina.m_RegisteredDepletionModifiers.Remove(STAMINA_MODIFIER);
            m_StaminaApplied = false;
        }
        // Also cover manual cycling and weapon swaps.
        Weapon_Base weapon = Weapon_Base.Cast(m_Player.GetHumanInventory().GetEntityInHands());
        if (weapon) COMOfflineAmmo.Refill(weapon, weapon.GetCurrentMuzzle());
    }
}