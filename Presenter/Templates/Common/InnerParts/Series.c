Symbol GetSymbol(string symbolCode)
{
	if (symbolCode == "0" || string.IsNullOrEmpty(symbolCode))
		symbolCode = Symbol.Code;
	return MarketData.GetSymbol(symbolCode);
}

MarketSeries GetSeries(string symbol, int period)
{
	var timeFrame = PeriodToTimeFrame(period);
	var symbolObject = GetSymbol(symbol);

	return MarketData.GetSeries(symbolObject.Code, timeFrame);
}

private DataSeries ToAppliedPrice(string symbol, int timeframe, int constant)
{
	var series = GetSeries(symbol, timeframe);
    switch (constant)
    {
        case PRICE_OPEN:
            return series.Open;
        case PRICE_HIGH:
            return series.High;
        case PRICE_LOW:
            return series.Low;
        case PRICE_CLOSE:
            return series.Close;
        case PRICE_MEDIAN:
            return series.Median;       
        case PRICE_TYPICAL:
            return series.Typical;    
        case PRICE_WEIGHTED:
            return series.Weighted;
    }
    throw new NotImplementedException("Converter doesn't support working with this type of AppliedPrice");
}   