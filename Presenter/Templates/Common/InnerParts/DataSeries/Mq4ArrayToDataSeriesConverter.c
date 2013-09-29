	class Mq4ArrayToDataSeriesConverter
	{
		private readonly Mq4Array<Mq4Double> _mq4Array;
		private readonly IndicatorDataSeries _dataSeries;

		public Mq4ArrayToDataSeriesConverter(Mq4Array<Mq4Double> mq4Array, IndicatorDataSeries dataSeries)
		{
			_mq4Array = mq4Array;
			_dataSeries = dataSeries;
			_mq4Array.Changed += OnValueChanged;
			CopyAllValues();
		}

		private void CopyAllValues()
		{
			for (var i = 0; i < _mq4Array.Length; i++)
			{
				if (_mq4Array.IsInverted)
					_dataSeries[_mq4Array.Length - i] = _mq4Array[i];
				else
					_dataSeries[i] = _mq4Array[i];
			}
		}

		private void OnValueChanged(int index, Mq4Double value)
		{			
			int indexToSet;
			if (_mq4Array.IsInverted)
				indexToSet = _mq4Array.Length - index;
			else
				indexToSet = index;

			if (indexToSet < 0)
				return;

			_dataSeries[indexToSet] = value;
		}
	}