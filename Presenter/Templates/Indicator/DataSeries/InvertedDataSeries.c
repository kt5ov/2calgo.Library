	internal class InvertedDataSeries
    {
        private DataSeries _dataSeries;
        private int _currentIndex;

        public InvertedDataSeries(DataSeries dataSeries)
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