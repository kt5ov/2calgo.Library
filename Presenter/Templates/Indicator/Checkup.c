	[Conditional("IsConnected")]
    bool IsConnected()
    {
        return true;
    }
	
	[Conditional("GetLastError")]
	int GetLastError()
	{
		return 0;
	}

