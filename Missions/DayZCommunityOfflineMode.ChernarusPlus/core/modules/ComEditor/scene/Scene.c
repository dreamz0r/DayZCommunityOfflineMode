class Scene
{
    ref array<Object> s_Objects = new array<Object>;
    ref SceneInfo sceneData;
    void Scene(SceneInfo data) { sceneData = data; }
    void ~Scene() { Clear(); }
    void Clear()
    {
        if (!GetGame()) return;
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.Clear);
        foreach (Object object : s_Objects)
            if (object) GetGame().ObjectDelete(object);
        s_Objects.Clear();
    }
}