class COMMapMenu extends PopupMenu
{
    protected MapWidget m_Map;
    protected TextWidget m_Status;
    protected TextWidget m_Title;
    override void Init()
    {
        m_Map = MapWidget.Cast(layoutRoot.FindAnyWidget("com_teleport_map"));
        m_Status = TextWidget.Cast(layoutRoot.FindAnyWidget("com_map_status"));
        m_Title = TextWidget.Cast(layoutRoot.FindAnyWidget("com_map_title"));
    }
    override void OnShow()
    {
        bool englishNames = GetGame().ConfigIsExisting("CfgPatches COMEnglishMapNames_" + GetGame().GetWorldName());
        if (englishNames) m_Title.SetText("Map teleport - satellite / English names");
        else m_Title.SetText("Map teleport - satellite / original names");
        if (m_Map && COM_GetPB())
        {
            m_Map.SetMapPos(COM_GetPB().GetPosition());
            m_Map.SetScale(0.3);
        }
        RefreshMarkers();
        if (englishNames) m_Status.SetText("Double-click to teleport. Escape closes this map.");
        else m_Status.SetText("English labels are not loaded. Enable EnglishMapNames and restart using a root deploy launcher.");
    }
    void RefreshMarkers()
    {
        if (!m_Map) return;
        m_Map.ClearUserMarks();
        if (COM_GetPB()) m_Map.AddUserMark(COM_GetPB().GetPosition(), "You", ARGB(255, 255, 255, 255), "\\dz\\gear\\navigation\\data\\map_tree_ca.paa");
    }
    bool TeleportAt(vector position)
    {
        COMKeyBinds keys = COMKeyBinds.Cast(COM_GetModuleManager().GetModule(COMKeyBinds));
        if (!keys || !keys.TeleportToPosition(position, true, false)) return false;
        RefreshMarkers();
        return true;
    }
    override bool OnClick(Widget w, int x, int y, int button)
    {
        if (!w || button != MouseState.LEFT) return false;
        if (w.GetName() == "com_map_center" && COM_GetPB()) { m_Map.SetMapPos(COM_GetPB().GetPosition()); return true; }
        return false;
    }
    override bool OnDoubleClick(Widget w, int x, int y, int button)
    {
        if (w != m_Map || button != MouseState.LEFT) return false;
        TeleportAt(m_Map.ScreenToMap(Vector(x, y, 0)));
        return true;
    }
}