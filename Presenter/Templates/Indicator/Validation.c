#region Validation
    void ValidateSymbolAndTimeFrame(string symbol, int timeframe)
    {
      if (timeframe != 0 && timeframe != Period())
                throw new NotImplementedException(NotSupportedDifferentTimeFrame);
    }
#endregion