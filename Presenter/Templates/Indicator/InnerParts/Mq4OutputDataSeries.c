class Mq4OutputDataSeries : IMq4Array<Mq4Double>
{
    public IndicatorDataSeries OutputDataSeries { get; private set; }
    private readonly IndicatorDataSeries _originalValues;
    private int _currentIndex;
    private int _shift;
    private double _emptyValue = double.NaN;
    private readonly ChartObjects _chartObjects;
	private readonly int _style;
	private readonly int _bufferIndex;
	private readonly #AlgoName_PLACE_HOLDER# _indicator;
		
    public Mq4OutputDataSeries(
		#AlgoName_PLACE_HOLDER# indicator, 
		IndicatorDataSeries outputDataSeries, 
		ChartObjects chartObjects, 
		int style, 
		int bufferIndex,
		Func<IndicatorDataSeries> dataSeriesFactory,
		int lineWidth,
		Colors? color = null)
    {
        OutputDataSeries = outputDataSeries;
        _chartObjects = chartObjects;
		_style = style;
		_bufferIndex = bufferIndex;
		_indicator = indicator;
		Color = color;
		_originalValues = dataSeriesFactory();
		LineWidth = lineWidth;
    }

	public int LineWidth { get; private set; }
	public Colors? Color { get; private set; }

    public int Length
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

    public Mq4Double this[int index]
    {
        get 
        { 
            var indexToGetFrom = _currentIndex - index + _shift;
            if (indexToGetFrom < 0 || indexToGetFrom >= _originalValues.Count)
                return 0;

            return _originalValues[_currentIndex - index + _shift];
        }
        set 
        { 
            var indexToSet = _currentIndex - index + _shift;  
			if (indexToSet < 0)
				return;

            _originalValues[indexToSet] = value;

            var valueToSet = value;
            if (valueToSet == _emptyValue)
                valueToSet = double.NaN;

            if (indexToSet < 0)
                return;
				
            OutputDataSeries[indexToSet] = valueToSet; 

			switch (_style)
			{
				case DRAW_ARROW:
					var arrowName = GetArrowName(indexToSet);
					if (double.IsNaN(valueToSet))
						_chartObjects.RemoveObject(arrowName);
					else
					{
						var color = Color.HasValue ? Color.Value : Colors.Red;
						_chartObjects.DrawText(arrowName , _indicator.ArrowByIndex[_bufferIndex], indexToSet, valueToSet, VerticalAlignment.Center, HorizontalAlignment.Center, color);
					}
					break;
				case DRAW_HISTOGRAM:
					if (#IsDrawingOnChartWindow_PLACE_HOLDER#)
					{
						var anotherLine = _indicator.AllBuffers.FirstOrDefault(b => b.LineWidth == LineWidth && b != this);
						if (anotherLine != null)
						{				
							var name = GetNameOfHistogramLineOnChartWindow(indexToSet);
							Colors color;
							if (this[index] > anotherLine[index])
								color = Color ?? Colors.Green;
							else
								color = anotherLine.Color ?? Colors.Green;
							var lineWidth = LineWidth;
							if (lineWidth != 1 && lineWidth < 5)
								lineWidth = 5;

							_chartObjects.DrawLine(name, indexToSet, this[index], indexToSet, anotherLine[index], color, lineWidth);
						}
					}
					break;
			}
        }
    }
		
	private string GetNameOfHistogramLineOnChartWindow(int index)
    {
		return string.Format("Histogram on chart window {0} {1}", LineWidth, index);
	}

    private string GetArrowName(int index)
    {
        return string.Format("Arrow {0} {1}", GetHashCode(), index);
    }
}

[Conditional("ArrayResize")]
int ArrayResize(Mq4OutputDataSeries array, int new_size)
{
	return array.Length;
}

[Conditional("ArraySetAsSeries")]
bool ArraySetAsSeries(Mq4OutputDataSeries mq4OutputDataSeries, bool value)
{
	return true;
}

[Conditional("iMAOnArray")]                
private double iMAOnArray(Mq4OutputDataSeries invertedDataSeries, int total, int period, int ma_shift, int ma_method, int shift) 
{
    return CalculateiMA(invertedDataSeries.OutputDataSeries, period, ma_method, shift + ma_shift);
} 

[Conditional("iRSIOnArray")]               
private double iRSIOnArray(Mq4OutputDataSeries invertedDataSeries, int total, int period, int shift) 
{
    return CalculateRsi(invertedDataSeries.OutputDataSeries, period, shift);
} 

[Conditional("iBandsOnArray")]                
private double iBandsOnArray(Mq4OutputDataSeries invertedDataSeries, int total, int period, int deviation, int bands_shift, int mode, int shift) 
{
	if (bands_shift != 0)
		throw new NotImplementedException(NotSupportedBandsShift);
            
	return CalculateBands(invertedDataSeries.OutputDataSeries, period, deviation, mode, shift);
}
     
[Conditional("iCCIOnArray")]     
private double iCCIOnArray(Mq4OutputDataSeries invertedDataSeries, int total, int period, int shift) 
{            
    return CalculateCCI(invertedDataSeries.OutputDataSeries, period, shift);
}

[Conditional("iStdDevOnArray")]                    
private double iStdDevOnArray(Mq4OutputDataSeries invertedDataSeries, int total, int ma_period, int ma_shift, int ma_method, int shift) 
{
    return CalculateiStdDev(invertedDataSeries.OutputDataSeries, ma_period, ma_shift, ma_method, shift);
}

[Conditional("SetIndexBuffer")]
bool SetIndexBuffer(int index, Mq4OutputDataSeries dataSeries) 
{
    return true;
}