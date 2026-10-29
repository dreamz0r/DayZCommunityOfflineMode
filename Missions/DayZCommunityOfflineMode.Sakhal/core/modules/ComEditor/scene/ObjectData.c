class ObjectData 
{
	string classname;
	vector position;
	vector orientation;

	void ObjectData(string name = "", vector pos = "0 0 0", vector ori = "0 0 0")
	{
		classname = name;
		position = pos;
		orientation = ori;
	}

	string GetClassName() 
	{
		return classname;
	}

	vector GetPosition() 
	{
		return position;
	}

	vector GetOrientation() 
	{
		return orientation;
	}
}