	class Mq4ArrayToDataSeriesAdapterFactory
	{
		private readonly Dictionary<Mq4Array<Mq4Double>, DataSeries> _cachedAdapters = new Dictionary<Mq4Array<Mq4Double>, DataSeries>();

		public DataSeries Create(Mq4Array<Mq4Double> mq4Array)
		{
			DataSeries result;

			if (_cachedAdapters.TryGetValue(mq4Array, out result))
				return result;

			result = new Mq4ArrayToDataSeriesAdapter(mq4Array);
			_cachedAdapters[mq4Array] = result;
			return result;
		}
	}