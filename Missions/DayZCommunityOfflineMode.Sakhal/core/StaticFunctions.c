static string COM_FormatFloat( float value, int decimals ) 
{
    if ( !value.ToString().Contains(".") ) return value.ToString();
	
    string result = "";
    array<string> output = new array<string>;

    value.ToString().Split(".", output);

    if ( output.Count() < 2 ) return value.ToString();

    if ( decimals <= 0 ) return output.Get(0);

    string fraction = output.Get(1);
    int length = Math.Min(decimals, fraction.Length());
    string right = fraction.Substring(0, length);
    result = output.Get(0) + "." + right;
    return result;
}

static string COM_VectorToString( vector vec )
{
    string result = vec.ToString();
    result.Replace( "<", "" );
    result.Replace( ">", "" );
    result.Replace( ",", "" );

    return result;
}

static string COM_VectorToString( vector vec, int decimals ) 
{
    string result = "";
    result = COM_FormatFloat(vec[0], decimals) + "|" + COM_FormatFloat(vec[1], decimals) + "|" + COM_FormatFloat(vec[2], decimals);

    return result;
}

static TStringArray COM_GetChildrenFromBaseClass( string strConfigName, string strBaseClass )
{
    string child_name = "";
    int count = GetGame().ConfigGetChildrenCount ( strConfigName );
    TStringArray class_names = new TStringArray;

    for (int p = 0; p < count; p++)
    {
        GetGame().ConfigGetChildName ( strConfigName, p, child_name );

        if ( GetGame().IsKindOf(child_name, strBaseClass ) && ( child_name != strBaseClass ) )
        {
            class_names.Insert(child_name);
        }
    }

    return class_names;
}

static TVectorArray COM_GetSpawnPoints()
{
    // Candidate locations from this terrain's vanilla fresh-spawn configuration.
    TVectorArray candidates = { "13994.366211 0 8075.476074", "14230.625977 0 9040.563477", "14616.412109 0 10927.664063", "9900.897461 0 13416.589844", "8737.237305 0 13614.225586", "13053.700195 0 12414.456055", "14709.013672 0 10496.27832", "14593.762695 0 9320.987305", "10993.058594 0 13249.263672", "14151.28125 0 8564.424805", "14637.454102 0 9853.299805", "12220.818359 0 12802.515625", "12559.836914 0 12601.771484", "10482.049805 0 13460.642578", "10193.813477 0 13449.232422", "9338.84375 0 13356.694336", "8473.584961 0 13533.678711", "8134.938965 0 13609.508789", "7781.99707 0 13476.615234", "11844.107422 0 12954.075195", "13132.085938 0 12122.124023", "14454.533203 0 9156.223633", "14278.936523 0 8777.439453", "11094.511719 0 13103.824219", "8594.275391 0 13653.040039", "14053.529297 0 11872.839844", "13239.754883 0 11867.707031", "13758.766602 0 8415.129883", "13899.277344 0 8582.242188", "14215.698242 0 11453.613281", "14055.693359 0 11583.552734", "11492.449219 0 13109.876953", "10740.030273 0 13132.52832", "13702.34668 0 7900.063477", "13530.728516 0 7458.29248", "13084.819336 0 7530.130371", "12755.78418 0 7064.543945", "12860.291992 0 7702.170898", "13428.8125 0 7629.255859" };
    TVectorArray points = new TVectorArray;
    foreach (vector position : candidates)
    {
        if (GetGame().SurfaceIsSea(position[0], position[2]) || GetGame().SurfaceIsPond(position[0], position[2])) continue;
        position[1] = GetGame().SurfaceY(position[0], position[2]) + 0.2;
        points.Insert(position);
    }
    if (points.Count() == 0)
    {
        vector fallback = candidates[0];
        fallback[1] = GetGame().SurfaceY(fallback[0], fallback[2]) + 0.2;
        points.Insert(fallback);
        Print("[COM] No dry-land spawn candidates; using the first vanilla spawn location.");
    }
    return points;
}

static set< Object > COM_GetObjectsAt( vector from, vector to, Object ignore = NULL, float radius = 0.5, Object with = NULL )
{
    vector contact_pos;
    vector contact_dir;
    int contact_component;

    set< Object > geom = new set< Object >;
    set< Object > view = new set< Object >;

    DayZPhysics.RaycastRV( from, to, contact_pos, contact_dir, contact_component, geom, with, ignore, false, false, ObjIntersectGeom, radius );
    DayZPhysics.RaycastRV( from, to, contact_pos, contact_dir, contact_component, view, with, ignore, false, false, ObjIntersectView, radius );

    if ( geom.Count() > 0 ) 
    {
        return geom;
    }
    if ( view.Count() > 0 ) 
    {
        return view;
    }
    return NULL;
}

static Object COM_GetPointerObject( Object ignore = NULL, float radius = 0.5, Object with = NULL )
{
    vector dir = GetGame().GetPointerDirection();

    vector from = GetGame().GetCurrentCameraPosition();

    vector to = from + ( dir * 10000 );

    auto objs = COM_GetObjectsAt( from, to, ignore, radius, with );

    if( objs && objs.Count() > 0 )
    {
        return objs[ 0 ];
    }

    return NULL;
}

static Object COM_GetCursorObject()
{
    vector rayStart = GetGame().GetCurrentCameraPosition();
    vector rayEnd = rayStart + GetGame().GetCurrentCameraDirection() * 10000;

    auto objs = COM_GetObjectsAt( rayStart, rayEnd );

    if( objs && objs.Count() > 0 )
    {
        return objs[ 0 ];
    }

    return NULL;
}

static vector COM_GetPointerPos()
{
    if ( !COM_GetPB() )
    {
        return "0 0 0";
    }

    vector dir = GetGame().GetPointerDirection();

    vector from = GetGame().GetCurrentCameraPosition();

    vector to = from + ( dir * 10000 );

    vector rayStart = from;
    vector rayEnd = to;
    vector hitPos;
    vector hitNormal;
    int hitComponentIndex;
    if (!DayZPhysics.RaycastRV(rayStart, rayEnd, hitPos, hitNormal, hitComponentIndex, NULL, NULL, COM_GetPB()))
        return COM_GetPB().GetPosition();

    return hitPos;
}

static bool COM_TryGetCursorPos(out vector hitPos)
{
    hitPos = vector.Zero;
    PlayerBase player = COM_GetPB();
    if (!player) return false;
    vector from = GetGame().GetCurrentCameraPosition();
    vector to = from + GetGame().GetCurrentCameraDirection() * 10000;
    vector normal;
    int component;
    return DayZPhysics.RaycastRV(from, to, hitPos, normal, component, NULL, NULL, player);
}

static vector COM_GetCursorPos()
{
    vector hitPos;
    if (COM_TryGetCursorPos(hitPos)) return hitPos;
    PlayerBase player = COM_GetPB();
    if (player) return player.GetPosition();
    return vector.Zero;
}
static void COM_Message( string txt ) 
{
    COM_GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", txt, ""));
}

static Weapon COM_GetWeaponInHands()
{
    Weapon weapon_in_hands;
    if( COM_GetPB() && COM_GetPB().GetItemInHands() ) Class.CastTo(weapon_in_hands,  COM_GetPB().GetItemInHands());

    return weapon_in_hands;
}

static MissionBase COM_GetMission()
{
    return MissionBase.Cast( GetGame().GetMission() );
}

static CommunityOfflineClient COM_GetClientMission()
{
    return CommunityOfflineClient.Cast( GetGame().GetMission() );
}

static CommunityOfflineServer COM_GetServerMission()
{
    return CommunityOfflineServer.Cast( GetGame().GetMission() );
}

static PlayerBase COM_GetPB()
{
    return PlayerBase.Cast( GetGame().GetPlayer() );
}

static bool COM_SHIFT()
{
    return( ( KeyState( KeyCode.KC_LSHIFT ) > 0 ) || ( KeyState( KeyCode.KC_RSHIFT ) > 0 ) );
}

static bool COM_CTRL()
{
    return( ( KeyState( KeyCode.KC_LCONTROL ) > 0 ) || ( KeyState( KeyCode.KC_RCONTROL ) > 0 ) );
}

static bool COM_ALT()
{
    return( ( KeyState( KeyCode.KC_LMENU ) > 0 ) || ( KeyState( KeyCode.KC_RMENU ) > 0 ) );
}

static bool COM_WINKEY()
{
    return( ( KeyState( KeyCode.KC_LWIN ) > 0 ) || ( KeyState( KeyCode.KC_RWIN ) > 0 ) );
}

/*
static Weapon_Base COM_CreateWeapon( PlayerBase oPlayer )
{
    Weapon_Base oWpn = Weapon_Base.Cast(oPlayer.GetInventory().CreateInInventory( "M4A1_Black" ));
    oWpn.GetInventory().CreateAttachment( "M4_Suppressor" );
    oWpn.GetInventory().CreateAttachment( "M4_RISHndgrd_Black" );
    oWpn.GetInventory().CreateAttachment( "M4_MPBttstck_Black" );
    oWpn.GetInventory().CreateAttachment( "ACOGOptic" );

    return oWpn;
}
*/

static Weapon_Base COM_CreateWeapon( PlayerBase oPlayer, string sWeapon )
{
    Weapon_Base oWpn = Weapon_Base.Cast(oPlayer.GetInventory().CreateInInventory( sWeapon ));
    if ( !oWpn ) return NULL;
    oWpn.GetInventory().CreateAttachment( "PistolSuppressor" );
    EntityAI optic = oWpn.GetInventory().CreateAttachment( "ReflexOptic" );
    if ( optic ) optic.GetInventory().CreateAttachment("Battery9V");

    return oWpn;
}

static PlayerBase COM_CreateCustomDefaultCharacter()
{
    PlayerBase oPlayer = PlayerBase.Cast( GetGame().CreatePlayer( NULL, GetGame().CreateRandomPlayer(), COM_GetSpawnPoints().GetRandomElement(), 0, "NONE") );

    if ( !oPlayer ) return NULL;

    // PlayerBase queues OnPlayerLoaded during creation; equip after that initialization.
    GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(COM_EquipStartingLoadout, oPlayer);


    return oPlayer;
}

static string COM_FileAttributeToString( FileAttr attr )
{
    string fileType = "";
    if ( attr & FileAttr.DIRECTORY )
    {
        fileType = fileType + "DIRECTORY";
    }
    if ( attr & FileAttr.HIDDEN )
    {
        fileType = fileType + "HIDDEN";
    }
    if ( attr & FileAttr.READONLY )
    {
        fileType = fileType + "READONLY";
    }
    if ( attr & FileAttr.INVALID )
    {
        fileType = fileType + "INVALID";
    }
    return fileType;
}

static vector COM_SnapToGround(vector pos)
{
    float pos_x = pos[0];
    float pos_z = pos[2];
    float pos_y = GetGame().SurfaceY( pos_x, pos_z );
    vector tmp_pos = Vector( pos_x, pos_y, pos_z );
    tmp_pos[1] = tmp_pos[1] + pos[1];

    return tmp_pos;
}

static bool m_COM_GodMode; // move these to player saves? Edit: Jacob says "yes"
static bool m_COM_OldAiming;
static bool COM_bc_Visible;

static void COM_SnapToGroundNew( Object object ) 
{
    vector pos = object.GetPosition();
    pos[1] = GetGame().SurfaceY(pos[0], pos[2]);
    
    vector clippingInfo[2];
    vector objectBBOX[2];
    
    object.GetCollisionBox( objectBBOX );
    object.ClippingInfo( clippingInfo );

    //float clipY = objectBBOX[1][1] / 2.0//- clippingInfo[0][1];
    //pos[1] = pos[1] + objectBBOX[1][1] - clipY;
    pos[1] = pos[1] + clippingInfo[1][1] / 2.0;//objectBBOX[0][1] - clipY

    object.SetPosition(pos);

    COM_ForceTargetCollisionUpdate( object );
}

static void COM_ForceTargetCollisionUpdate( Object oObj )
{
    if ( !oObj ) return;

    vector roll = oObj.GetOrientation();
    roll [ 2 ] = roll [ 2 ] - 1;
    oObj.SetOrientation( roll );
    roll [ 2 ] = roll [ 2 ] + 1;
    oObj.SetOrientation( roll );
}

static void COM_ToggleCursor()
{
    if ( GetGame().GetInput().HasGameFocus( INPUT_DEVICE_MOUSE ) )
    {
        GetGame().GetInput().ChangeGameFocus( 1 );
        GetGame().GetUIManager().ShowUICursor( true );
    }
    else
    {
        GetGame().GetUIManager().ShowUICursor( false );
        GetGame().GetInput().ResetGameFocus();
    }
}

/*
    Token types:
     0 - error, no token
     1 - defined token (special characters etc. . / * )
     2 - quoted string. Quotes are removed -> TODO
     3 - alphabetic string
     4 - number
     5 - end of line -> TODO
*/
static bool COM_CheckStringType( string str, int type ) 
{
    for(int i = 0; i<str.Length(); i++ ) 
    {
        string character = str.Get(i);
        string token;
        int result = character.ParseStringEx(token);
        if ( result == type ) return true;
    }
    return false;
}

string COM_GetRandomChildFromBaseClass( string strConfigName, string strBaseClass, int minScope = -1 )
{
    string child_name = "";
    int count = GetGame().ConfigGetChildrenCount ( strConfigName );
    array< string > class_names = new array<string>;

    for ( int p = 0; p < count; p++ )
    {
        GetGame().ConfigGetChildName( strConfigName, p, child_name );

        if( ( minScope != -1 ) && ( GetGame().ConfigGetInt( strConfigName + " " + child_name + " scope" ) < minScope ) ) continue;

        if ( GetGame().IsKindOf( child_name, strBaseClass ) && ( child_name != strBaseClass ) )
        {
            class_names.Insert( child_name );
        }
    }

    if (class_names.Count() == 0) return "";
    return class_names.GetRandomElement();
}