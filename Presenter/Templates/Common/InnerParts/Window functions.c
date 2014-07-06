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
	RefreshData();
	return true;
}

[Conditional("WindowExpertName")]
Mq4String WindowExpertName()
{
    return "#AlgoName_PLACE_HOLDER#";
}

[Conditional("HideTestIndicators")]
//{
void HideTestIndicators(Mq4Double hide)
{
}

void HideTestIndicators()
{
}
//}
