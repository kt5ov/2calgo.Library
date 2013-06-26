	#region Predefined variables

    private int Bars
    {
        get { return MarketSeries.Close.Count; }
    }

    private int Digits
    {
		get
		{
			if (Symbol == null)
				return 0;
			return Symbol.Digits;
		}
    }
      
	Mq4Double Point
	{
		get
		{
			if (Symbol == null)
				return 0.00001;

			return Symbol.PointSize;
		}
	}

	private int? _period;
	private int Period()
	{         
		if (_period == null)
		{
			var counters = new Dictionary<int, int>();
			for (var i = 1; i < MarketSeries.Close.Count - 1; i++)
			{
			var timeSpan = (int)(MarketSeries.OpenTime[i] - MarketSeries.OpenTime[i - 1]).TotalMinutes;
			if (!counters.ContainsKey(timeSpan))
				counters.Add(timeSpan, 0);
			counters[timeSpan]++;
			}
			_period = counters.OrderByDescending(kvp => kvp.Value)
					.Select(kvp => kvp.Key)
					.First();
			_period = Math.Min(_period.Value, 43200);
		}
		return _period.Value;
	}
#endregion