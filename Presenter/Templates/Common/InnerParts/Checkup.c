[Conditional("IsConnected")]
bool IsConnected()
{
    return true;
}
	
private int _lastError;

[Conditional("GetLastError")]
int GetLastError()
{
	return _lastError;
}

[Conditional("IsTesting", "Alert")]
bool IsTesting()
{
	return false;
}

[Conditional("IsStopped")]
bool IsStopped()
{
	return false;
}

[Conditional("UninitializeReason")]
int UninitializeReason()
{
	return REASON_REMOVE;
}

