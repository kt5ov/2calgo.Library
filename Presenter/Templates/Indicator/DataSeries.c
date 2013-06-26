	#region DataSeries

    internal class Mq4TimeSeries
    {
        private readonly TimeSeries _timeSeries;
        private int _currentIndex;
        private static readonly DateTime StartDateTime = new DateTime(1970, 1, 1);

        public Mq4TimeSeries(TimeSeries timeSeries)
        { 
            _timeSeries = timeSeries;
        }

        public void SetCurrentIndex(int index)
        {
            _currentIndex = index;
        }       

        public static int ToInteger(DateTime dateTime)
        {
          return (int)(dateTime - StartDateTime).TotalSeconds;
        }
            
        public static DateTime ToDateTime(int seconds)
        {
          return StartDateTime.AddSeconds(seconds);
        }
                
        public int this[int index]
        {
            get 
            { 
                if (index >= _timeSeries.Count)
                  return 0;
                  
                DateTime dateTime = _timeSeries[_currentIndex - index];

                return ToInteger(dateTime); 
            }
        }
    }

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

    class Mq4DataSeries
    {
        public IndicatorDataSeries OutputDataSeries { get; private set; }
        private readonly NormalIndicatorDataSeries _originalValues = new NormalIndicatorDataSeries();
        private int _currentIndex;
        private int _shift;
        private double _emptyValue = double.NaN;
        private readonly DataSeriesExtremums _closeExtremums;
        private readonly ChartObjects _chartObjects;
        private readonly List<int> _overlapLineStartIndexes = new List<int>();

        public Mq4DataSeries(IndicatorDataSeries outputDataSeries, DataSeriesExtremums closeExtremums, ChartObjects chartObjects)
        {
            OutputDataSeries = outputDataSeries;
            _closeExtremums = closeExtremums;
            _chartObjects = chartObjects;
        }

        public int Count
        {
            get 
            {
                return OutputDataSeries.Count;
            }
        }

        public void SetCurrentIndex(int index)
        {
            _currentIndex = index;
        }        
        
        public void SetShift(int shift)
        {
          _shift = shift;
        }
        
        public void SetEmptyValue(double emptyValue)
        {
          _emptyValue = emptyValue;
        }

        public double this[int index]
        {
            get 
            { 
                if (index >= _originalValues.Count)
                  return 0;
                        
                return _originalValues[_currentIndex - index + _shift];
            }
            set 
            { 
                var indexToSet = _currentIndex - index + _shift;                
                _originalValues[indexToSet] = value;

                var valueToSet = value;
                if (valueToSet == _emptyValue)
                  valueToSet = double.NaN;

                if (indexToSet < 0)
                  return;

                if (#IsDrawingOnChartWindow_PLACE_HOLDER#)
                {
                    var validRange = _closeExtremums.Max - _closeExtremums.Min;                
                    if (value > _closeExtremums.Max + validRange || value < _closeExtremums.Min - validRange)
                        return;
                }

                if (!double.IsNaN(valueToSet) && double.IsNaN(OutputDataSeries[indexToSet - 1]))
                {
                    int startIndex;
                    for (startIndex = indexToSet - 1; startIndex >= 0; startIndex--)
                    {
                        if (!double.IsNaN(OutputDataSeries[startIndex]))
                            break;
                    }
                    if (startIndex > 0)
                    {
                        RemoveOverlapLinesSinceIndex(startIndex);

                        _chartObjects.DrawLine(GetOverlapLineName(startIndex), startIndex, OutputDataSeries[startIndex], indexToSet, valueToSet, Colors.Black, 3);
                        _overlapLineStartIndexes.Add(startIndex);
                    }                    
                }

                OutputDataSeries[indexToSet] = valueToSet; 
            }
        }

        private void RemoveOverlapLinesSinceIndex(int index)
        {
            foreach (var startIndex in _overlapLineStartIndexes.ToArray())                    
            {
                if (startIndex >= index)
                {
                    _overlapLineStartIndexes.Remove(startIndex);
                    _chartObjects.RemoveObject(GetOverlapLineName(startIndex));
                }
            }
        }

        private string GetOverlapLineName(int startIndex)
        {
            return string.Format("Overlapline {0} {1}", GetHashCode(), startIndex);
        }
    }

    internal class NormalIndicatorDataSeries : IndicatorDataSeries
    {
      private List<double> _data = new List<double>();
      
      private void EnsureCountIsEnough(int index)
      {
        while (_data.Count <= index)
          _data.Add(double.NaN);
      }
      
      public double this[int index]
      {       
        get
        {
          if (index < 0)
            return double.NaN;    
            
          EnsureCountIsEnough(index);         
          
          return _data[index];
        }       
        set
        {
          if (index < 0)
            return;
            
        EnsureCountIsEnough(index);
          
          _data[index] = value;
        }
      }
      
      public int Count 
      {
        get
      {
        return _data.Count;     
      }
      }
      
      public double LastValue
      {
        get
        {
          return this[Count - 1];
        }
      }
    }

#endregion //DataSeries