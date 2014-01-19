	class Mq4ArrayToDataSeriesConverterFactory
	{
		private readonly Dictionary<Mq4DoubleArray, IndicatorDataSeries> _cachedAdapters = new Dictionary<Mq4DoubleArray, IndicatorDataSeries>();
		private Func<IndicatorDataSeries> _dataSeriesFactory;

		public Mq4ArrayToDataSeriesConverterFactory(Func<IndicatorDataSeries> dataSeriesFactory)
		{
			_dataSeriesFactory = dataSeriesFactory;
		}

		public DataSeries Create(Mq4DoubleArray mq4Array)
		{
			IndicatorDataSeries dataSeries;

			if (_cachedAdapters.TryGetValue(mq4Array, out dataSeries))
				return dataSeries;

			dataSeries = _dataSeriesFactory();
			new Mq4ArrayToDataSeriesConverter(mq4Array, dataSeries);
			_cachedAdapters[mq4Array] = dataSeries;

			return dataSeries;
		}
	}