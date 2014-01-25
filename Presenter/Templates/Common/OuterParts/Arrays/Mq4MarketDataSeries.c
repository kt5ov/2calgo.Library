	class Mq4MarketDataSeries : IMq4DoubleArray
    {
        private DataSeries _dataSeries;

        public Mq4MarketDataSeries(DataSeries dataSeries)
        {
            _dataSeries = dataSeries;
        }

        public Mq4Double this[int index]
        {
            get { return _dataSeries.Last(index); }
			set { }
        }

		public int Length 
		{
			get { return _dataSeries.Count; }
		}

        public void Resize(int newSize)
        {        
        }
    }