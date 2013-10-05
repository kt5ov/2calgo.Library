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

