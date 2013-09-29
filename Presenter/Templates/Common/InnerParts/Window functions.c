	[Conditional("WindowFind")]
    int WindowFind(Mq4String name)
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
