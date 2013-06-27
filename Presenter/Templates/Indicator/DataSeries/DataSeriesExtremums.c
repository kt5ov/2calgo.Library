	class DataSeriesExtremums
    {
        private int? _lastCheckedIndex;
        private readonly DataSeries _dataSeries;
        private double _min = double.MaxValue;
        private double _max = double.MinValue;

        public DataSeriesExtremums(DataSeries dataSeries)
        {
            _dataSeries = dataSeries;
        }

        private void UpdateMinAndMax()
        {
            var indexFrom = _lastCheckedIndex != null ? _lastCheckedIndex.Value + 1 : 0;
            for (var i = indexFrom; i < _dataSeries.Count - 1; i++)
            {
                if (_dataSeries[i] < _min)
                    _min = _dataSeries[i];
                if (_dataSeries[i] > _max)
                    _max = _dataSeries[i];
                _lastCheckedIndex = i;
            }
        }

        public double Min
        {
            get 
            {
                UpdateMinAndMax();
                return _min;
            }
        }

        public double Max
        {
            get 
            {
                UpdateMinAndMax();
                return _max;
            }
        }
    }