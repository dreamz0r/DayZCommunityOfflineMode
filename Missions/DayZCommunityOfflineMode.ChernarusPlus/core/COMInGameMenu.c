// Offline Restart replaces the character without tearing down the running economy.
class COMInGameMenu extends InGameMenu
{
    void COMInGameMenu() { SetID(MENU_INGAME); }
    override int GetID() { return MENU_INGAME; }
    override Widget Init()
    {
        Widget root = super.Init();
        if (CommunityOfflineClient.Cast(GetGame().GetMission()) && !GetGame().IsMultiplayer())
            ButtonSetText(m_RestartButton, "Respawn");
        return root;
    }
    override protected void OnClick_Restart()
    {
        CommunityOfflineClient offline = CommunityOfflineClient.Cast(GetGame().GetMission());
        if (offline && !GetGame().IsMultiplayer()) { offline.RequestOfflineRespawn(); Close(); return; }
        super.OnClick_Restart();
    }
    override protected void OnClick_Respawn()
    {
        CommunityOfflineClient offline = CommunityOfflineClient.Cast(GetGame().GetMission());
        if (offline && !GetGame().IsMultiplayer()) { offline.RequestOfflineRespawn(); Close(); return; }
        super.OnClick_Respawn();
    }
}
