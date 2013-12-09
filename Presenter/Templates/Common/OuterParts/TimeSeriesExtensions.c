	static class TimeSeriesExtensions
	{
		public static DateTime Last(this TimeSeries timeSeries, int index)
		{
			return timeSeries[timeSeries.InvertIndex(index)];
		}

		public static int InvertIndex(this TimeSeries timeSeries, int index)
		{
			return timeSeries.Count - 1 - index;
		}

		public static int GetIndexByTime(this TimeSeries timeSeries, DateTime time)
		{
			var index = timeSeries.Count - 1;
			for (var i = timeSeries.Count - 1; i >= 0; i--)
			{
				if (timeSeries[i] < time)
				{
					index = i + 1;
					break;
				}
			}   
			return index;
		}
	}