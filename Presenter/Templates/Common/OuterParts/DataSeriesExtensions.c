	static class DataSeriesExtensions
	{
		public static int InvertIndex(this DataSeries dataSeries, int index)
		{
			return dataSeries.Count - 1 - index;
		}
	}