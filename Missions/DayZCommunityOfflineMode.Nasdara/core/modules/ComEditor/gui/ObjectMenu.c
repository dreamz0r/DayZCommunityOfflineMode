class ObjectMenu extends PopupMenu
{
	protected TextListboxWidget m_classList;
	protected EditBoxWidget m_SearchBox;
	protected ButtonWidget m_btnSpawnGround;
	protected ButtonWidget m_btnSpawnCursor;
	protected ButtonWidget m_btnSpawnInventory;
	protected ButtonWidget m_btnCancel;
	protected EditBoxWidget m_QuantityItem;
	protected EditBoxWidget m_StateItem;

	private ItemPreviewWidget m_item_widget;
	protected EntityAI previewItem;
    protected bool m_PreviewVisible;
    protected string m_PendingPreviewClass;
    protected string m_PreviewClass;
	private int m_characterRotationX;
	private int m_characterRotationY; // Borrowed from inspectmenu
	private int m_characterScaleDelta;
	private vector m_characterOrientation;

	protected EditBoxWidget m_editBox;

	protected autoptr array<string> m_excludeBroken = {
	    "GP25Base",
	    "GP25",
	    "GP25_Standalone",
	    "M203Base",
	    "M203",
	    "M203_Standalone",
	    "ItemOptics",
    };

    void ~ObjectMenu()
    {
        ClearPreview();
    }

    // Only inventory objects belong in ItemPreviewWidget. Creating characters,
    // AI, effects or world structures here runs their native world initialization.
    static bool IsPreviewSupported(string className)
    {
        if (className == "") return false;
        if (GetGame().IsKindOf(className, "Man") || GetGame().IsKindOf(className, "DZ_LightAI")) return false;
        return GetGame().IsKindOf(className, "Inventory_Base") || GetGame().ConfigIsExisting(CFG_WEAPONSPATH + " " + className) || GetGame().ConfigIsExisting(CFG_MAGAZINESPATH + " " + className);
    }

    void ClearPreview()
    {
        if (!GetGame()) return;
        GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.LoadPendingPreview);
        GetGame().GetDragQueue().RemoveCalls(this);
        m_PendingPreviewClass = "";
        m_PreviewClass = "";
        if (m_item_widget) m_item_widget.SetItem(NULL);
        if (previewItem) GetGame().ObjectDelete(previewItem);
        previewItem = NULL;
    }

    void LoadPendingPreview()
    {
        string className = m_PendingPreviewClass;
        m_PendingPreviewClass = "";
        if (!m_PreviewVisible || className != GetCurrentSelection() || !IsPreviewSupported(className)) return;
        if (m_excludeBroken.Find(className) != -1) return;
        if (!m_item_widget)
        {
            Widget panel = layoutRoot.FindAnyWidget("preview_pnl");
            if (!panel) return;
            m_item_widget = ItemPreviewWidget.Cast(GetGame().GetWorkspace().CreateWidget(ItemPreviewWidgetTypeID, 0, 0, 1, 1, WidgetFlags.VISIBLE, ARGB(255, 255, 255, 255), 10, panel));
        }
        if (!m_item_widget) return;
        previewItem = EntityAI.Cast(GetGame().CreateObjectEx(className, vector.Zero, ECE_LOCAL | ECE_NOLIFETIME | ECE_NOPERSISTENCY_WORLD | ECE_NOPERSISTENCY_CHAR));
        if (!previewItem) return;
        previewItem.DisableSimulation(true);
        previewItem.SetAllowDamage(false);
        dBodyDestroy(previewItem);
        m_PreviewClass = className;
        m_characterOrientation = vector.Zero;
        m_item_widget.SetItem(previewItem);
        m_item_widget.SetModelPosition("0 0 0.5");
        m_item_widget.SetModelOrientation(m_characterOrientation);
        m_item_widget.SetSize(1.5, 1.5);
        m_item_widget.SetPos(-0.225, -0.225);
        Magazine magazine = Magazine.Cast(previewItem);
        ItemBase item = ItemBase.Cast(previewItem);
        if (magazine) m_QuantityItem.SetText(magazine.GetAmmoMax().ToString());
        else if (item) m_QuantityItem.SetText(item.GetQuantityMax().ToString());
    }
	override void Init()
	{
		m_classList = TextListboxWidget.Cast( layoutRoot.FindAnyWidget( "classlist" ) );
		m_SearchBox = EditBoxWidget.Cast( layoutRoot.FindAnyWidget( "search_input" ) );
		m_btnSpawnGround = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_spawn_ground" ) );
		m_btnSpawnCursor = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_spawn_cursorpos" ) );
		m_btnSpawnInventory = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_spawn_inventory" ) );
		m_btnCancel = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_cancel" ) );

		m_QuantityItem = EditBoxWidget.Cast( layoutRoot.FindAnyWidget( "quantity_items" ) );
		m_StateItem = EditBoxWidget.Cast( layoutRoot.FindAnyWidget( "state_items" ) );
		
		m_editBox = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("className_spawner_box"));
	}

	override void OnShow()
    {
        m_PreviewVisible = true;
        UpdateList("All");
    }

    override void OnHide()
    {
        m_PreviewVisible = false;
        ClearPreview();
    }

	override bool OnChange( Widget w, int x, int y, bool finished )
    {
        if (w == m_editBox) { ClearPreview(); m_classList.SelectRow(-1); return true; }
        if ( w == m_SearchBox )
        {
            UpdateList( "All" );
            return true;
        }

        return false;
    }

    override bool OnMouseEnter( Widget w , int x, int y )
    {
        if ( w == m_SearchBox )
        {
            COM_GetPB().GetInputController().OverrideMovementSpeed( true, 0 );
        }
        return false;
    }

    override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
    {
        if ( w == m_SearchBox )
        {
            COM_GetPB().GetInputController().OverrideMovementSpeed( false, 0 );
        }
        return false;
    }

    override bool OnClick( Widget w, int x, int y, int button )
    {
        if (!w) return false;
        if (w.GetName().Contains("btn_filter"))
        {
            string buttonName = w.GetName();
            buttonName.Replace("btn_filter_", "");
            UpdateList(buttonName);
            return true;
        }

        if ((w != m_btnSpawnCursor) && (w != m_btnSpawnGround) && (w != m_btnSpawnInventory))
            return true;

        if (!COM_GetPB()) return true;
        string strSelection = GetCurrentSelection();
        if (!strSelection)
            strSelection = GetEditBoxInput();
        
        if (!strSelection)
            return true;
          
        if( m_excludeBroken.Find(strSelection) != -1 ) 
            return true;
        
        // Keep the configured class name intact for modded types and diagnostics.

        ObjectEditor obEditor = ObjectEditor.Cast(COM_GetModuleManager().GetModule(ObjectEditor));

        EntityAI spawned;
        if (w == m_btnSpawnInventory)
        {
            spawned = g_Game.GetPlayer().GetInventory().CreateInInventory( strSelection );
        }
        else
        {
            vector position = COM_GetPB().GetPosition();
            if (w == m_btnSpawnCursor)
              position = COM_GetCursorPos();

            bool isAi = GetGame().IsKindOf(strSelection, "DZ_LightAI");
            spawned = EntityAI.Cast(g_Game.CreateObject(strSelection, position, true, isAi));
            if ( !spawned )
            {
                COM_Message( "Could not spawn: " + strSelection );
                return true;
            }
            spawned.PlaceOnSurface();
            COM_ForceTargetCollisionUpdate(spawned);

            obEditor.addObject(spawned);
            ObjectInfoMenu.listBox.AddItem(spawned.GetType(), spawned, 0);
        }

        if (!spawned) { COM_Message("Could not spawn into inventory: " + strSelection); return true; }
        ItemBase spawnedItem = ItemBase.Cast(spawned);
        if (spawnedItem)
        {
            float quantity01 = 1;
            float maximum = spawnedItem.GetQuantityMax();
            Magazine spawnedMagazine = Magazine.Cast(spawnedItem);
            if (spawnedMagazine) maximum = spawnedMagazine.GetAmmoMax();
            if (maximum > 0) quantity01 = Math.Clamp(m_QuantityItem.GetText().ToFloat(), 0, maximum) / maximum;
            float health = m_StateItem.GetText().ToFloat();
            SetupSpawnedItem(spawnedItem, health, quantity01);
        }

        return true;
    }

    override bool OnItemSelected(Widget w, int x, int y, int row, int column, int oldRow, int oldColumn)
    {
        if (w != m_classList) return false;
        string className = GetCurrentSelection();
        if (className != "" && (className == m_PendingPreviewClass || className == m_PreviewClass)) return true;
        ClearPreview();
        float maximum = 1;
        if (GetGame().ConfigIsExisting(CFG_MAGAZINESPATH + " " + className)) maximum = GetGame().ConfigGetFloat(CFG_MAGAZINESPATH + " " + className + " count");
        else if (GetGame().ConfigIsExisting(CFG_VEHICLESPATH + " " + className + " varQuantityMax")) maximum = GetGame().ConfigGetFloat(CFG_VEHICLESPATH + " " + className + " varQuantityMax");
        m_QuantityItem.SetText(maximum.ToString());
        if (!m_PreviewVisible || !IsPreviewSupported(className) || m_excludeBroken.Find(className) != -1) return true;
        m_PendingPreviewClass = className;
        // Coalesce scrolling/key-repeat events; only the final settled row is loaded.
        GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.LoadPendingPreview, 250, false);
        return true;
    }
	override bool OnMouseButtonDown( Widget w, int x, int y, int button ) 
	{
		if (m_PreviewVisible && previewItem && m_item_widget && w == m_item_widget)
		{
			GetGame().GetDragQueue().Call(this, "UpdateRotation");
			GetMousePos(m_characterRotationX, m_characterRotationY);
			return true;
		}
		return false;
	}

	override bool OnMouseWheel( Widget w, int x, int y, int wheel ) 
	{
		if (m_PreviewVisible && previewItem && m_item_widget && w == m_item_widget)
		{
			GetGame().GetDragQueue().Call(this, "UpdateScale");
			m_characterScaleDelta = wheel ;
		}
		return false;
	}

	void UpdateScale(int mouse_x, int mouse_y, int wheel, bool is_dragging) // Borrowed from inspect menu
	{
		if (!m_PreviewVisible || !previewItem || !m_item_widget) return;
        float w, h, x, y;
		m_item_widget.GetPos(x, y);
		m_item_widget.GetSize(w,h);
		w = w + ( m_characterScaleDelta / 4);
		h = h + ( m_characterScaleDelta / 4 );
		if ( w > 0.5 && w < 4 )
		{
			m_item_widget.SetSize( w, h );
	
			// align to center 
			int screen_w, screen_h;
			GetScreenSize(screen_w, screen_h);
			float new_x = x - ( m_characterScaleDelta / 8 );
			float new_y = y - ( m_characterScaleDelta / 8 );
			m_item_widget.SetPos( new_x, new_y );
		}
	}

	void UpdateRotation(int mouse_x, int mouse_y, bool is_dragging) // Borrowed from inspect menu
	{
        if (!m_PreviewVisible || !previewItem || !m_item_widget) return;
        vector o = m_characterOrientation;
		o[0] = o[0] + (m_characterRotationY - mouse_y);
		o[1] = o[1] - (m_characterRotationX - mouse_x);
		
		m_item_widget.SetModelOrientation( o );
		
		if (!is_dragging)
		{
			m_characterOrientation = o;
		}
	}

    void UpdateList( string classFilter ) // All default
    {
        ClearPreview();
        m_classList.ClearItems();

		TStringArray configs = new TStringArray;
		configs.Insert( CFG_VEHICLESPATH );
		configs.Insert( CFG_WEAPONSPATH );
		configs.Insert( CFG_MAGAZINESPATH );

        string strSearch = m_SearchBox.GetText();

		strSearch.ToLower();

		for ( int nConfig = 0; nConfig < configs.Count(); ++nConfig )
		{
			string strConfigPath = configs.Get( nConfig );

			int nClasses = g_Game.ConfigGetChildrenCount( strConfigPath );

			for ( int nClass = 0; nClass < nClasses; ++nClass )
			{
				string strName;

				g_Game.ConfigGetChildName( strConfigPath, nClass, strName );

				int scope = g_Game.ConfigGetInt( strConfigPath + " " + strName + " scope" );

				if ( scope < 2 )
				{
					continue;
				}

				auto strNameLower = strName;

				strNameLower.ToLower();

				if ( GetGame().IsKindOf( strNameLower, classFilter ) || classFilter == "All" ) // Fix for weapon_base not being child of "All"
				{
					if ( (strSearch != "" && (!strNameLower.Contains( strSearch ))) ) 
					{
						continue;
					}

					m_classList.AddItem( strName, NULL, 0 );
				}
			}
		}
    }

	string GetCurrentSelection()
	{
		if (m_classList && m_classList.GetSelectedRow() >= 0 && m_classList.GetSelectedRow() < m_classList.GetNumItems())
		{
			string result;
			m_classList.GetItemText( m_classList.GetSelectedRow(), 0, result );
			return result;
		}

		return "";
	}
	
	string GetEditBoxInput() 
	{
		return m_editBox.GetText();
	}
}