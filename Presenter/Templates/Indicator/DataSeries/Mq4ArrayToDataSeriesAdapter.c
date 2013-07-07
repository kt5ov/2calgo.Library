#region Mq4ArrayToDataSeriesAdapter

	class Mq4ArrayToDataSeriesAdapter : DataSeries
	{
		private readonly IMq4Array<Mq4Double> _mq4Array;
		public Mq4ArrayToDataSeriesAdapter(IMq4Array<Mq4Double> mq4Array)
		{
			_mq4Array = mq4Array;
		}

		public double LastValue 
		{ 
			get 
			{
				if (_mq4Array.IsInverted)
					return _mq4Array[0];
				return _mq4Array[_mq4Array.Length - 1];
			}
		}

		public double this[int index]
		{
			get 
			{
				if (_mq4Array.IsInverted)
					return _mq4Array[_mq4Array.Length - index];
				return  _mq4Array[index];
			}
		}
		
		public int Count
		{
			get
			{
				return _mq4Array.Length;
			}
		}
	}

#endregion //Mq4ArrayToDataSeriesAdapter