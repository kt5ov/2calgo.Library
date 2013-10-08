[Conditional("WindowFind")]
Mq4Double WindowFind(Mq4String name)
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
Mq4Double RefreshRates()
{
	return true;
}

[Conditional("WindowExpertName")]
Mq4String WindowExpertName()
{
    return "#AlgoName_PLACE_HOLDER#";
}

