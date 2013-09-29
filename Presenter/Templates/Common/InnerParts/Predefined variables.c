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
		if (TimeFrame == TimeFrame.Minute10)
			return 10;
		if (TimeFrame == TimeFrame.Minute15)
			return 15;
		if (TimeFrame == TimeFrame.Minute30)
			return 30;
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

	public TimeFrame PeriodToTimeFrame(int period)
    {
        switch (period) 
		{		
			case 0: 			
				return TimeFrame;
            case 1:
                return TimeFrame.Minute;
            case 2:
                return TimeFrame.Minute2;
            case 3:
                return TimeFrame.Minute3;
            case 4:
                return TimeFrame.Minute4;
            case 5:
                return TimeFrame.Minute5;
            case 10:
                return TimeFrame.Minute10;
            case 15:
                return TimeFrame.Minute15;
            case 30:
                return TimeFrame.Minute30;
            case 60:
                return TimeFrame.Hour;
            case 240:
                return TimeFrame.Hour4;
            case 720:
                return TimeFrame.Hour12;
            case 1440:
                return TimeFrame.Daily;
            case 10080:
                return TimeFrame.Weekly;
            case 43200:
                return TimeFrame.Monthly;
			default:
				throw new NotSupportedException(string.Format("TimeFrame {0} minutes isn't supported by cAlgo", period));
        }
    }