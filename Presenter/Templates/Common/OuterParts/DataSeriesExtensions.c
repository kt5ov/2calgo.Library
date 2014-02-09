	static class DataSeriesExtensions
	{
		public static int InvertIndex(this DataSeries dataSeries, int index)
		{
			return dataSeries.Count - 1 - index;
		}

		public static Mq4Double Last(this DataSeries dataSeries, int shift, DataSeries sourceDataSeries)
		{
			return dataSeries[sourceDataSeries.Count - 1 - shift];
		}
	}