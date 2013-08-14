	[Conditional("WindowFind")]
    int WindowFind(string name)
    {
        return 0;
    }

	[Conditional("WindowRedraw")]
    void WindowRedraw()
    {
    }

	[Conditional("ObjectsRedraw")]
    void ObjectsRedraw()
    {
    }
		
	[Conditional("RefreshRates")]
	bool RefreshRates()
	{
		return true;
	}
