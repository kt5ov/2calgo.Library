[Conditional("MarketInfo")]
Mq4Double MarketInfo(string symbol, int type)
{
	var symbolObject = GetSymbol(symbol);
	switch (type)
	{
		case MODE_LOW:
			return GetSeries(symbol, PERIOD_D1).Low.LastValue;
		case MODE_HIGH:
			return GetSeries(symbol, PERIOD_D1).High.LastValue;
		case MODE_DIGITS:
			return symbolObject.Digits;
		case MODE_TIME:
			return TimeCurrent();
		case MODE_ASK:
			return symbolObject.Ask;
		case MODE_BID:
			return symbolObject.Bid;
		case MODE_SPREAD:
			return symbolObject.Spread / symbolObject.PointSize;
		case MODE_PROFITCALCMODE:
			return 0;
		case MODE_FREEZELEVEL:
			return 0;
		case MODE_TRADEALLOWED:
			return 1;
		case MODE_POINT:
			return symbolObject.PointSize;
		case MODE_TICKSIZE:
			return symbolObject.PointSize;
		case MODE_SWAPTYPE:
			return 0;
		case MODE_MARGINCALCMODE:
			return 0;
	}
	return 0;
}

[Conditional("Bid", "MarketInfo")]
Mq4Double Bid
{
    get 
    {
    if (Symbol == null || double.IsNaN(Symbol.Bid))
        return 0;
    return Symbol.Bid;
    }
}

[Conditional("Ask", "MarketInfo")]
Mq4Double Ask
{
    get 
    {
    if (Symbol == null || double.IsNaN(Symbol.Ask))
        return 0;
    return Symbol.Ask;
    }
}

[Conditional("Comment")]
void Comment(params object[] objects)
{
    var text = string.Join("", objects.Select(o => o.ToString()));
    ChartObjects.DrawText("top left comment", text, StaticPosition.TopLeft);
}

[Conditional("Sleep")]
void Sleep(int milliseconds)
{        
}

[Conditional("PlaySound")]
void PlaySound(string fileName)
{
    Notifications.PlaySound(fileName);
}
	
[Conditional("SendMail")]
void SendMail(string subject, string text)
{
    Notifications.SendEmail(EmailAddressFrom, EmailAddressTo, subject, text);
}

[Conditional("Print")]
void Mq4Print(params object[] parameters)
{
	Print(string.Join(string.Empty, parameters));
}