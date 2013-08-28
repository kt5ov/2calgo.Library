	class Mq4ArrayToDataSeriesConverterFactory
	{
		private readonly Dictionary<Mq4Array<Mq4Double>, IndicatorDataSeries> _cachedAdapters = new Dictionary<Mq4Array<Mq4Double>, IndicatorDataSeries>();
		private Func<IndicatorDataSeries> _dataSeriesFactory;

		public Mq4ArrayToDataSeriesConverterFactory(Func<IndicatorDataSeries> dataSeriesFactory)
		{
			_dataSeriesFactory = dataSeriesFactory;
		}

		public DataSeries Create(Mq4Array<Mq4Double> mq4Array)
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