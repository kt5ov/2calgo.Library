	[Conditional("iHighest", "iLowest", "Lowest", "Highest")]
    int GetHighestIndex(DataSeries dataSeries, int count, int invertedStart)
    {
		var start = invertedStart;
		var maxIndex = start;
		var endIndex = count == WHOLE_ARRAY ? (dataSeries.Count - 1) : (count + start - 1);
		for (var i = start; i <= endIndex; i++)
		{
			if (dataSeries.FromEnd(i) > dataSeries.FromEnd(maxIndex))
				maxIndex = i;
		}
		return maxIndex;
    }

	[Conditional("iHighest", "iLowest", "Lowest", "Highest")]
	int GetLowestIndex(DataSeries dataSeries, int count, int invertedStart)
	{
		var start = invertedStart;
		var minIndex = start;
		var endIndex = count == WHOLE_ARRAY ? (dataSeries.Count - 1) : (count + start - 1);
		for (var i = start; i <= endIndex; i++)
		{
			if (dataSeries.FromEnd(i) < dataSeries.FromEnd(minIndex))
				minIndex = i;
		}
		return minIndex;
	}

	[Conditional("iHighest", "iLowest", "Lowest", "Highest")]
    int GetExtremeIndex(Func<DataSeries, int, int, int> extremeFunc, string symbol, int timeframe, int type, int count, int start)
    {
		var marketSeries = GetSeries(symbol, timeframe);
		switch (type)
		{
			case MODE_OPEN:
				return extremeFunc(marketSeries.Open, count, start);         
			case MODE_HIGH:
				return extremeFunc(marketSeries.High, count, start);
			case MODE_LOW:
				return extremeFunc(marketSeries.Low, count, start);
			case MODE_CLOSE:
				return extremeFunc(marketSeries.Close, count, start);
			case MODE_VOLUME:
				return extremeFunc(marketSeries.TickVolume, count, start);
			case MODE_TIME:
				return start;
			default:
				throw new ArgumentOutOfRangeException("wrong type for GetExtremeIndex");
		}
    }

	[Conditional("iHighest", "Highest")]
    //{
	int iHighest(Mq4String symbol, int timeframe, int type, int count = WHOLE_ARRAY, int start = 0)
    {
      return GetExtremeIndex(GetHighestIndex, symbol, timeframe, type, count, start);
    }
	
    int Highest(Mq4String symbol, int timeframe, int type, int count = WHOLE_ARRAY, int start = 0)
    {
      return iHighest(symbol, timeframe, type, count, start);
    }
	//}

	[Conditional("iLowest", "Lowest")]
	//{
    int iLowest(Mq4String symbol, int timeframe, int type, int count = WHOLE_ARRAY, int start = 0)
    {
      return GetExtremeIndex(GetLowestIndex, symbol, timeframe, type, count, start);
    }

    int Lowest(Mq4String symbol, int timeframe, int type, int count = WHOLE_ARRAY, int start = 0)
    {
      return iLowest(symbol, timeframe, type, count, start);
    }
	//}
	
	[Conditional("iOpen")]
    Mq4Double iOpen(Mq4String symbol, int timeframe, int shift)
    {	
		return GetSeries(symbol, timeframe).Open.FromEnd(shift);
    }

	[Conditional("iHigh")]
    Mq4Double iHigh(Mq4String symbol, int timeframe, int shift)
    {
		return GetSeries(symbol, timeframe).High.FromEnd(shift);
    }

	[Conditional("iLow")]
    Mq4Double iLow(Mq4String symbol, int timeframe, int shift)
    {
		return GetSeries(symbol, timeframe).Low.FromEnd(shift);
    }
		
	[Conditional("iClose")]
    Mq4Double iClose(Mq4String symbol, int timeframe, int shift)
    {
		return GetSeries(symbol, timeframe).Close.FromEnd(shift);
    }
	
	[Conditional("iVolume")]
    Mq4Double iVolume(Mq4String symbol, int timeframe, int shift)
    {
		return GetSeries(symbol, timeframe).TickVolume.FromEnd(shift);
    }

	[Conditional("iTime")]
    int iTime(Mq4String symbol, int timeframe, int shift)
    {
		var timeSeries = GetSeries(symbol, timeframe).OpenTime;
		if (shift >= 0 && shift < timeSeries.Count)
			return Mq4TimeSeries.ToInteger(timeSeries.FromEnd(shift));
		return 0;
    }

	[Conditional("iBarShift")]
    int iBarShift(Mq4String symbol, int timeframe, int time, bool exact = false)
    {
		var marketSeries = GetSeries(symbol, timeframe);
		var dateTime = Mq4TimeSeries.ToDateTime(time);
		for (var i = marketSeries.Close.Count - 1; i >= 0; i--)
		{
			if (marketSeries.OpenTime[i] == dateTime)
				return marketSeries.OpenTime.InvertIndex(i);
			if (marketSeries.OpenTime[i] < dateTime && !exact)
				return marketSeries.OpenTime.InvertIndex(i);
		}
		return -1;
    }

	[Conditional("iBars")]
	int iBars(string symbol, int timeframe)
	{
		return GetSeries(timeframe).Close.Count;
	}

	