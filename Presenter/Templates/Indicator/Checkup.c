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

[Conditional("IsTesting")]
bool IsTesting()
{
	return false;
}


