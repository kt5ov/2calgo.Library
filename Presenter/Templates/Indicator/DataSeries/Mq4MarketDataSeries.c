	internal class Mq4MarketDataSeries
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

        public double this[int index]
        {
            get { return _dataSeries[_currentIndex - index]; }
        }
    }