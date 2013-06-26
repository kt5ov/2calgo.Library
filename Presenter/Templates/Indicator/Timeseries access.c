#region Timeseries access

    int GetHighestIndex(InvertedDataSeries invertedDataSeries, int count, int start)
    {
      var maxIndex = start;
      var endIndex = count == WHOLE_ARRAY ? Bars - 1 : count + start - 1;
      for (var i = start; i <= endIndex; i++)
      {
        if (invertedDataSeries[i] > invertedDataSeries[maxIndex])
          maxIndex = i;
      }
      return maxIndex;
    }

    int GetLowestIndex(InvertedDataSeries invertedDataSeries, int count, int start)
    {
      var minIndex = start;
      var endIndex = count == WHOLE_ARRAY ? Bars - 1 : count + start - 1;
      for (var i = start; i <= endIndex; i++)
      {
        if (invertedDataSeries[i] < invertedDataSeries[minIndex])
          minIndex = i;
      }
      return minIndex;
    }

    int GetExtremeIndex(Func<InvertedDataSeries, int, int, int> extremeFunc, int type, int count, int start)
    {
      switch (type)
      {
        case MODE_OPEN:
          return extremeFunc(Open, count, start);         
        case MODE_HIGH:
          return extremeFunc(High, count, start);
        case MODE_LOW:
          return extremeFunc(Low, count, start);
        case MODE_CLOSE:
          return extremeFunc(Close, count, start);
        case MODE_VOLUME:
          return extremeFunc(Volume, count, start);
        case MODE_TIME:
          return start;
        default:
          throw new ArgumentOutOfRangeException("wrong type for GetExtremeIndex");
      }
    }

    int iHighest(string symbol, int timeframe, int type, int count = WHOLE_ARRAY, int start = 0)
    {
      ValidateSymbolAndTimeFrame(symbol, timeframe);
      return GetExtremeIndex(GetHighestIndex, type, count, start);
    }

    int iLowest(string symbol, int timeframe, int type, int count = WHOLE_ARRAY, int start = 0)
    {
      ValidateSymbolAndTimeFrame(symbol, timeframe);
      return GetExtremeIndex(GetLowestIndex, type, count, start);
    }

    int Highest(string symbol, int timeframe, int type, int count = WHOLE_ARRAY, int start = 0)
    {
      return iHighest(symbol, timeframe, type, count, start);
    }

    int Lowest(string symbol, int timeframe, int type, int count = WHOLE_ARRAY, int start = 0)
    {
      return iLowest(symbol, timeframe, type, count, start);
    }

    double iClose(string symbol, int timeframe, int shift)
    {
      ValidateSymbolAndTimeFrame(symbol, timeframe);
      return Close[shift];
    }

    double iHigh(string symbol, int timeframe, int shift)
    {
      ValidateSymbolAndTimeFrame(symbol, timeframe);
      return High[shift];
    }

    double iLow(string symbol, int timeframe, int shift)
    {
      ValidateSymbolAndTimeFrame(symbol, timeframe);
      return Low[shift];
    }

    double iOpen(string symbol, int timeframe, int shift)
    {
      ValidateSymbolAndTimeFrame(symbol, timeframe);
      return Open[shift];
    }

    double iVolume(string symbol, int timeframe, int shift)
    {
      ValidateSymbolAndTimeFrame(symbol, timeframe);
      return Volume[shift];
    }

    int iTime(string symbol, int timeframe, int shift)
    {
      ValidateSymbolAndTimeFrame(symbol, timeframe);
      return Time[shift];
    }


    int iBarShift(string symbol, int timeframe, int time, bool exact = false)
    {
      ValidateSymbolAndTimeFrame(symbol, timeframe);
      for (var i = 0; i < MarketSeries.Close.Count; i++)
      {
        if (Time[i] == time)
          return i;
        if (Time[i] < time && !exact)
          return i;
      }
      return -1;
    }
#endregion