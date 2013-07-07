	class Mq4OutputDataSeries : IMq4Array<Mq4Double>
    {
        public cAlgo.API.IndicatorDataSeries NotInvertedDataSeries { get; private set; }
        private readonly IndicatorDataSeries _originalValues = new IndicatorDataSeries();
        private int _currentIndex;
        private int _shift;
        private double _emptyValue = double.NaN;
        private readonly DataSeriesExtremums _closeExtremums;
        private readonly ChartObjects _chartObjects;
        private readonly List<int> _overlapLineStartIndexes = new List<int>();
		private readonly int _style;
		private readonly int _bufferIndex;
		private readonly ConvertedIndicator _indicator;
		private readonly Colors? _color;

        public Mq4OutputDataSeries(
			ConvertedIndicator indicator, 
			cAlgo.API.IndicatorDataSeries outputDataSeries, 
			DataSeriesExtremums closeExtremums, 
			ChartObjects chartObjects, 
			int style, 
			int bufferIndex,
			Colors? color = null)
        {
            NotInvertedDataSeries = outputDataSeries;
            _closeExtremums = closeExtremums;
            _chartObjects = chartObjects;
			_style = style;
			_bufferIndex = bufferIndex;
			_indicator = indicator;
			_color = color;
        }

        public int Length
        {
            get 
            {
                return NotInvertedDataSeries.Count;
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

        public Mq4Double this[int index]
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
					
				switch (_style)
				{
					case DRAW_LINE:
						if (!double.IsNaN(valueToSet) && double.IsNaN(NotInvertedDataSeries[indexToSet - 1]))
						{
							int startIndex;
							for (startIndex = indexToSet - 1; startIndex >= 0; startIndex--)
							{
								if (!double.IsNaN(NotInvertedDataSeries[startIndex]))
									break;
							}
							if (startIndex > 0)
							{
								RemoveOverlapLinesSinceIndex(startIndex);

								_chartObjects.DrawLine(GetOverlapLineName(startIndex), startIndex, NotInvertedDataSeries[startIndex], indexToSet, valueToSet, Colors.Black, 3);
								_overlapLineStartIndexes.Add(startIndex);
							}                    
						}
						break;
					case DRAW_ARROW:
						var arrowName = GetArrowName(indexToSet);
						if (double.IsNaN(valueToSet))
							_chartObjects.RemoveObject(arrowName);
						else
						{
							var color = _color.HasValue ? _color.Value : Colors.Red;
							_chartObjects.DrawText(arrowName , _indicator.ArrowByIndex[_bufferIndex], indexToSet, valueToSet, VerticalAlignment.Center, HorizontalAlignment.Center, color);
						}
						break;
				}

                NotInvertedDataSeries[indexToSet] = valueToSet; 
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
		
        private string GetArrowName(int index)
        {
            return string.Format("Arrow {0} {1}", GetHashCode(), index);
        }
    }