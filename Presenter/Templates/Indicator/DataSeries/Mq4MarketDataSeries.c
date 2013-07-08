	class Mq4MarketDataSeries : IMq4Array<Mq4Double>
    {
        private DataSeries _dataSeries;
        private int _currentIndex;

        public Mq4MarketDataSeries(DataSeries dataSeries)
        {
            _dataSeries = dataSeries;
        }

        public void SetCurrentIndex(int index)
        {
            _currentIndex = index;
        }

        public Mq4Double this[int index]
        {
            get { return _dataSeries[_currentIndex - index]; }
			set { }
        }

		public int Length 
		{
			get { return _dataSeries.Count; }
		}
    }