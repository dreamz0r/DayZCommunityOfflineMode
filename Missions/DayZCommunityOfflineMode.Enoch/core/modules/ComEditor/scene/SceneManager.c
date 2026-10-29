string SCENE_DIR = "$profile:SceneManager";
string SCENE_ACTIVE = SCENE_DIR + "\\ActiveScenes";
string SCENE_DATA = SCENE_DIR + "\\Scenes";

class SceneManager
{
    ref map<ref SceneInfo, ref array<ref Scene>> sm_Scenes = new map<ref SceneInfo, ref array<ref Scene>>;

    void SceneManager()
    {
        MakeDirectory(SCENE_DIR);
        MakeDirectory(SCENE_DATA);
        MakeDirectory(SCENE_ACTIVE);
        Init();
    }
    void ~SceneManager()
    {
        UnloadScenes();
    }
    void Init()
    {
        LoadScenes();
    }
    void UnloadScenes()
    {
        foreach (SceneInfo data, array<ref Scene> instances : sm_Scenes)
            foreach (Scene instance : instances)
                if (instance) instance.Clear();
        sm_Scenes.Clear();
    }
    static bool IsValidFileName(string name)
    {
        return name != "" && name.IndexOf("..") == -1 && name.IndexOf("/") == -1 && name.IndexOf("\\") == -1 && name.IndexOf(":") == -1;
    }
    void LoadScenes()
    {
        UnloadScenes();
        FileAttr attributes;
        string fileName;
        FindFileHandle handle = FindFile(SCENE_DATA + "\\*.json", fileName, attributes, FindFileFlags.ALL);
        if (fileName != "")
        {
            bool more = true;
            while (more)
            {
                if (!(attributes & FileAttr.DIRECTORY))
                {
                    SceneInfo data = LoadSceneData(fileName);
                    if (data) sm_Scenes.Insert(data, new array<ref Scene>);
                }
                more = FindNextFile(handle, fileName, attributes);
            }
        }
        CloseFindFile(handle);
    }
    SceneInfo LoadSceneData(string fileName)
    {
        if (!IsValidFileName(fileName)) return NULL;
        if (fileName.Length() < 5 || fileName.Substring(fileName.Length() - 5, 5) != ".json") fileName += ".json";
        SceneInfo data;
        string error;
        if (!FileExist(SCENE_DATA + "\\" + fileName)) return NULL;
        if (!JsonFileLoader<SceneInfo>.LoadFile(SCENE_DATA + "\\" + fileName, data, error) || !data || data.name == "" || !data.m_objects)
        {
            Print("[COM] Cannot load scene " + fileName + ": " + error);
            return NULL;
        }
        return data;
    }
    bool SaveSceneData(string fileName, SceneInfo data)
    {
        if (!IsValidFileName(fileName) || !data || data.name == "") return false;
        if (fileName.Length() < 5 || fileName.Substring(fileName.Length() - 5, 5) != ".json") fileName += ".json";
        string error;
        if (!JsonFileLoader<SceneInfo>.SaveFile(SCENE_DATA + "\\" + fileName, data, error))
        {
            Print("[COM] Cannot save scene: " + error);
            return false;
        }
        return true;
    }
    Scene ActivateScene(SceneInfo data)
    {
        if (!data || !data.m_objects) return NULL;
        if (!sm_Scenes.Contains(data)) sm_Scenes.Insert(data, new array<ref Scene>);
        Scene instance = new Scene(data);
        foreach (ObjectData entry : data.m_objects)
        {
            if (!entry || entry.classname == "") continue;
            Object object = GetGame().CreateObjectEx(entry.classname, entry.position, ECE_SETUP | ECE_CREATEPHYSICS | ECE_KEEPHEIGHT);
            if (!object) { Print("[COM] Unavailable scene object: " + entry.classname); continue; }
            object.SetOrientation(entry.orientation);
            instance.s_Objects.Insert(object);
        }
        sm_Scenes.Get(data).Insert(instance);
        if (!data.persistent && data.duration > 0) GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(instance.Clear, data.duration * 1000, false);
        return instance;
    }
}