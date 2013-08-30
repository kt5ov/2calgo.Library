	static class DataSeriesExtensions
	{
		public static Mq4Double FromEnd(this DataSeries dataSeries, int index)
		{
			return dataSeries[dataSeries.InvertIndex(index)];
		}

		public static int InvertIndex(this DataSeries dataSeries, int index)
		{
			return dataSeries.Count - 1 - index;
		}
	}