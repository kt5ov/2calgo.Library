    private int Bars
    {
        get { return MarketSeries.Close.Count; }
    }

	[Conditional("Digits", "MarketInfo")]
    private int Digits
    {
		get
		{
			if (Symbol == null)
				return 0;
			return Symbol.Digits;
		}
    }
    
	[Conditional("Point")]
	Mq4Double Point
	{
		get
		{
			if (Symbol == null)
				return 0.00001;

			return Symbol.PointSize;
		}
	}

	private int Period()
	{         
		if (TimeFrame == TimeFrame.Minute)
			return 1;
		if (TimeFrame == TimeFrame.Minute2)
			return 2;
		if (TimeFrame == TimeFrame.Minute3)
			return 3;
		if (TimeFrame == TimeFrame.Minute4)
			return 4;
		if (TimeFrame == TimeFrame.Minute5)
			return 5;
		if (TimeFrame == TimeFrame.Hour)
			return 60;
		if (TimeFrame == TimeFrame.Hour4)
			return 240;
		if (TimeFrame == TimeFrame.Hour12)
			return 720;                    
		if (TimeFrame == TimeFrame.Daily)
			return 1440;
		if (TimeFrame == TimeFrame.Weekly)
			return 10080;
		
		return 43200;
	}