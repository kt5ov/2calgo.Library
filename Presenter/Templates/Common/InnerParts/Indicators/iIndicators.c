
class ParametersKey
{
	private readonly object[] _parameters;
	
	public ParametersKey(params object[] parameters)
	{
		_parameters = parameters;		
	}
	
	public override bool Equals(object obj)
	{
		var other = (ParametersKey)obj;
		for (var i = 0; i < _parameters.Length; i++)
		{
			if (!_parameters[i].Equals(other._parameters[i]))
				return false;
		}
		return true;
	}
	
	public override int GetHashCode()
	{
		unchecked
		{
			var hashCode = 0;
			foreach (var parameter in _parameters)
			{
				hashCode = (hashCode*397) ^ parameter.GetHashCode();
			}
			return hashCode;
		}
	}
}

class Cache<TValue>
{
	private Dictionary<ParametersKey, TValue> _dictionary = new Dictionary<ParametersKey, TValue>();
	
	public bool TryGetValue(out TValue value, params object[] parameters)
	{
		var key = new ParametersKey(parameters);
		return _dictionary.TryGetValue(key, out value);
	}
	
	public void Add(TValue value, params object[] parameters)
	{
		var key = new ParametersKey(parameters);
		_dictionary.Add(key, value);
	}
}


        private static MovingAverageType ToMaType(int constant)
        {
            switch (constant)
            {
                case MODE_SMA:
                    return MovingAverageType.Simple;
                case MODE_EMA:
                    return MovingAverageType.Exponential;
                case MODE_LWMA:
                    return MovingAverageType.Weighted;
                default:
                  throw new ArgumentOutOfRangeException("Not supported moving average type");
            }
        }

		[Conditional("iMA", "iMAOnArray", "iBearsPower", "iBullsPower", "iForce")]
		//{
#region iMA
        private double iMA(Mq4String symbol, Mq4Double timeframe, Mq4Double period, Mq4Double ma_shift, Mq4Double ma_method, Mq4Double applied_price, Mq4Double shift)
        {
            var series = ToAppliedPrice(symbol, timeframe, applied_price);
      
            return CalculateiMA(series, period, ma_method, shift + ma_shift);
        }       
                
        private double iMAOnArray(Mq4DoubleArray mq4Array, Mq4Double total, Mq4Double period, Mq4Double ma_shift, Mq4Double ma_method, Mq4Double shift) 
        {
			var dataSeries = _mq4ArrayToDataSeriesConverterFactory.Create(mq4Array);
			return CalculateiMA(dataSeries, period, ma_method, shift + ma_shift);
        }
        
        private double CalculateiMA(DataSeries dataSeries, int period, int ma_method, int shift)
        {     
            if (ma_method == MODE_SMMA)
            {
                return CalculateWellesWilderSmoothing(dataSeries, period, shift);
            }

            var maType = ToMaType(ma_method);            
            var indicator = _cachedStandardIndicators.MovingAverage(dataSeries, period, maType);

            return indicator.Result.Last(shift, dataSeries);
        }        
        
        private double CalculateWellesWilderSmoothing(DataSeries dataSeries, int period, int shift)
        {
            var indicator = _cachedStandardIndicators.WellesWilderSmoothing(dataSeries, period);
            
            return indicator.Result.Last(shift);
        }
#endregion //iMA
		//}

		[Conditional("iRSI", "iRSIOnArray")]
		//{
#region iRSI
        private Mq4Double iRSI(Mq4String symbol, Mq4Double timeframe, Mq4Double period, Mq4Double applied_price, Mq4Double shift)
        {
            var series = ToAppliedPrice(symbol, timeframe, applied_price);
      
            return CalculateRsi(series, period, shift);
        }       
                
        private Mq4Double iRSIOnArray(Mq4DoubleArray mq4Array, Mq4Double total, Mq4Double period, Mq4Double shift) 
        {
			var dataSeries = _mq4ArrayToDataSeriesConverterFactory.Create(mq4Array);
			return CalculateRsi(dataSeries, period, shift);
        }
        
        private double CalculateRsi(DataSeries dataSeries, int period, int shift)
        {     
            var indicator = Indicators.RelativeStrengthIndex(dataSeries, period);
            return indicator.Result.Last(shift);
        }

#endregion //iRSI    
		//}

		[Conditional("iBands", "iBandsOnArray")]
		//{
#region iBands
        private Mq4Double iBands(Mq4String symbol, Mq4Double timeframe, Mq4Double period, Mq4Double deviation, Mq4Double bands_shift, Mq4Double applied_price, Mq4Double mode, Mq4Double shift)
        {            
            var series = ToAppliedPrice(symbol, timeframe, applied_price);
      
            return CalculateBands(series, period, deviation, mode, bands_shift + shift);
        }       
        
        private double CalculateBands(DataSeries dataSeries, int period, int deviation, int mode, int shift)
        {     
            var indicator = Indicators.BollingerBands(dataSeries, period, deviation, MovingAverageType.Simple);

            switch (mode)
            {
              case MODE_MAIN:
                return indicator.Main.Last(shift);
              case MODE_UPPER:
                return indicator.Top.Last(shift);
              case MODE_LOWER:
                return indicator.Bottom.Last(shift);
            }

            return 0;
        }

#endregion //iBands   		
		//}

		[Conditional("iADX")]
		//{
#region iADX
        private Mq4Double iADX(Mq4String symbol, Mq4Double timeframe, Mq4Double period, Mq4Double applied_price, Mq4Double mode, Mq4Double shift)
        {
			var marketSeries = GetSeries(symbol, timeframe);
      
            return CalculateAdx(marketSeries, period, mode, shift);
        }      
        
        private Mq4Double CalculateAdx(MarketSeries marketSeries, int period, int mode, int shift)
        {     
            var indicator = Indicators.DirectionalMovementSystem(marketSeries, period);            
            switch (mode)
            {
              case MODE_MAIN:
                return indicator.ADX.Last(shift);
              case MODE_PLUSDI:
                return indicator.DIPlus.Last(shift);
              case MODE_MINUSDI:
                return indicator.DIMinus.Last(shift);
            }
            return 0;
        }

#endregion //iADX    
		//}

		[Conditional("iATR")]
		//{
#region iATR
        private Mq4Double iATR(Mq4String symbol, Mq4Double timeframe, Mq4Double period, Mq4Double shift)
        {
			var series = GetSeries(symbol, timeframe);
            return CalculateATR(series, period, shift);
        }       
                
        private double CalculateATR(MarketSeries series, int period, int shift)
        {     
            var indicator = _cachedStandardIndicators.ATR(series, period);

            return indicator.Result.Last(shift);            
        }

#endregion //iATR   
		//}

[Conditional("iMACD")]
//{
        private Mq4Double iMACD(Mq4String symbol, Mq4Double timeframe, Mq4Double fast_ema_period, Mq4Double slow_ema_period, Mq4Double signal_period, Mq4Double applied_price, Mq4Double mode, Mq4Double shift)
        {
            var series = ToAppliedPrice(symbol, timeframe, applied_price);
      
            var indicator = Indicators.MacdCrossOver(series, fast_ema_period, slow_ema_period, signal_period);

            switch (mode)
            {
              case MODE_MAIN:
                return indicator.MACD.Last(shift);
              default:
                return indicator.Signal.Last(shift);
            }
        }       
//}
[Conditional("iOsMA")]
//{
        private Mq4Double iOsMA(Mq4String symbol, Mq4Double timeframe, Mq4Double fast_ema_period, Mq4Double slow_ema_period, Mq4Double signal_period, Mq4Double applied_price, Mq4Double shift)
        {
            var series = ToAppliedPrice(symbol, timeframe, applied_price);
      
            var indicator = Indicators.MacdCrossOver(series, fast_ema_period, slow_ema_period, signal_period);

            return indicator.Histogram.Last(shift);
        }       
//}

		[Conditional("iCCI", "iCCIOnArray")]
		//{
#region iCCI
        private Mq4Double iCCI(Mq4String symbol, Mq4Double timeframe, Mq4Double period, Mq4Double applied_price, Mq4Double shift)
        {
            var series = ToAppliedPrice(symbol, timeframe, applied_price);
      
            return CalculateCCI(series, period, shift);
        }       
        
        private Cache<CciIndicator_> _cciIndicators = new Cache<CciIndicator_>();
        private double CalculateCCI(DataSeries dataSeries, int period, int shift)
        {     
            CciIndicator_ indicator = null;
            if (!_cciIndicators.TryGetValue(out indicator, dataSeries, period))
			{
                indicator = new CciIndicator_(dataSeries, period, Indicators);
                _cciIndicators.Add(indicator, dataSeries, period);
			}
            
            var index = dataSeries.Count - 1 - shift;
            return indicator.Calculate(index);
        }

        class CciIndicator_
        {
            private readonly DataSeries _source;
            private readonly int _period;
            private readonly SimpleMovingAverage _sma;

            public CciIndicator_(DataSeries source, int period, IIndicatorsAccessor indicatorsAccessor)
            {
                _source = source;
                _period = period;
                _sma = indicatorsAccessor.SimpleMovingAverage(_source, _period);
            }

            public double Calculate(int index)
            {
                var meanDeviation = 0.0;
                for (var i = (index - _period); i < index; i++)
                {
                    meanDeviation = meanDeviation + Math.Abs(_source[i] - _sma.Result[index]);
                }
                meanDeviation /= _period;
                return (_source[index] - _sma.Result[index]) / (meanDeviation * 0.015);
            }
        }

        
#endregion //iCCI		
		//}

		[Conditional("iStdDev", "iStdDevOnArray")]
		//{
#region iStdDev
        private Mq4Double iStdDev(Mq4String symbol, Mq4Double timeframe, Mq4Double ma_period, Mq4Double ma_shift, Mq4Double ma_method, Mq4Double applied_price, Mq4Double shift)
        {
            if (ma_shift != 0)
                throw new NotImplementedException(NotSupportedMaShift);

            var series = ToAppliedPrice(symbol, timeframe, applied_price);
      
            return CalculateiStdDev(series, ma_period, ma_shift, ma_method, shift);
        }       
        
        private double CalculateiStdDev(DataSeries dataSeries, int ma_period, int ma_shift, int ma_method, int shift)
        {     
            if (ma_shift != 0)
                throw new Exception(NotSupportedMaShift);

            var maType = ToMaType(ma_method);            
            var indicator = Indicators.StandardDeviation(dataSeries, ma_period, maType);

            return indicator.Result.Last(shift);
        }        
#endregion //iStdDev		
		//}

		[Conditional("iWPR")]
		//{
#region iWPR
        private Mq4Double iWPR(Mq4String symbol, Mq4Double timeframe, Mq4Double period, Mq4Double shift)
        {
			var marketSeries = GetSeries(symbol, timeframe);
            var indicator = Indicators.WilliamsPctR(marketSeries, period);

            return indicator.Result.Last(shift);
        }        
#endregion //iWPR		
		//}

		[Conditional("iSAR")]
		//{
#region iSAR
        private Mq4Double iSAR(Mq4String symbol, Mq4Double timeframe, Mq4Double step, Mq4Double maximum, Mq4Double shift)
        {
			var series = GetSeries(symbol, timeframe);
            var indicator = Indicators.ParabolicSAR(series, step, maximum);

            return indicator.Result.Last(shift);
        }        
#endregion //iSAR
		//}

[Conditional("iFractals")]
//{
	private Mq4Double iFractals(Mq4String symbol, Mq4Double timeframe, Mq4Double mode, Mq4Double shift)
	{
		var index = _currentIndex - shift;
		var marketSeries = GetSeries(symbol, timeframe);
		if (mode == MODE_UPPER)
		{
			if (IsUpFractal(marketSeries, index))
				return marketSeries.High[index];
		}
		if (mode == MODE_LOWER)
		{
			if (IsDownFractal(marketSeries, index))
				return marketSeries.Low[index];
		}
		return 0;
	}
		
	private static bool IsUpFractal(MarketSeries marketSeries, int i)
    {
        var high = marketSeries.High;
        if (high[i] > high[i - 1] && high[i] > high[i - 2] && high[i] > high[i + 1] && high[i] > high[i + 2])
            return true;

        if (high[i] == high[i - 1] && high[i] > high[i - 2] && high[i] > high[i - 3] && high[i] > high[i + 1] && high[i] > high[i + 2])
            return true;

        if (high[i] >= high[i - 1] && high[i] == high[i - 2] && high[i] > high[i - 3] && high[i] > high[i - 4] && high[i] > high[i + 1] && high[i] > high[i + 2])
            return true;

        if (high[i] >= high[i - 1] && high[i] == high[i - 2] && high[i] == high[i - 3] && high[i] > high[i - 4] && high[i] > high[i - 5] && high[i] > high[i + 1] && high[i] > high[i + 2])
            return true;

        if (high[i] >= high[i - 1] && high[i] == high[i - 2] && high[i] >= high[i - 3] && high[i] == high[i - 4] && high[i] > high[i - 5] && high[i] > high[i - 6] && high[i] > high[i + 1] && high[i] > high[i + 2])
            return true;

        return false;
    }

    private static bool IsDownFractal(MarketSeries marketSeries, int i)
    {
        var low = marketSeries.Low;
        if (low[i] < low[i - 1] && low[i] < low[i - 2] && low[i] < low[i + 1] && low[i] < low[i + 2])
            return true;

        if (low[i] == low[i - 1] && low[i] < low[i - 2] && low[i] < low[i - 3] && low[i] < low[i + 1] && low[i] < low[i + 2])
            return true;

        if (low[i] <= low[i - 1] && low[i] == low[i - 2] && low[i] < low[i - 3] && low[i] < low[i - 4] && low[i] < low[i + 1] && low[i] < low[i + 2])
            return true;

        if (low[i] <= low[i - 1] && low[i] == low[i - 2] && low[i] == low[i - 3] && low[i] < low[i - 4] && low[i] < low[i - 5] && low[i] < low[i + 1] && low[i] < low[i + 2])
            return true;

        if (low[i] <= low[i - 1] && low[i] == low[i - 2] && low[i] <= low[i - 3] && low[i] == low[i - 4] && low[i] < low[i - 5] && low[i] < low[i - 6] && low[i] < low[i + 1] && low[i] < low[i + 2])
            return true;

        return false;
    }

//}

[Conditional("iStochastic")]
//{

	Mq4Double iStochastic(Mq4String symbol, Mq4Double timeframe, Mq4Double kperiod, Mq4Double dperiod, Mq4Double slowing, Mq4Double method, Mq4Double price_field, Mq4Double mode, Mq4Double shift)
	{
		var maType = ToMaType(method);   
		var stochasticMode = method == 0 ? StochasticMode.LowHigh : StochasticMode.CloseClose;		
		var marketSeries = GetSeries(symbol, timeframe);
        var index = marketSeries.Close.Count - 1 - shift;

		var stochasticValues = CalculateStochastic(marketSeries, kperiod, dperiod, slowing, maType, stochasticMode, index);

		switch (mode)
		{
			case MODE_MAIN:
				return stochasticValues.K;
			case MODE_SIGNAL:
				return stochasticValues.D; 
		}

		return 0;
	}

	private struct StochasticParameters
    {
        public int KPeriods;
		public int DPeriods;
		public int KSlowing;
		public MovingAverageType MAType;
		public StochasticMode StochasticMode;
		public MarketSeries MarketSeries;

		public bool Equals(StochasticParameters other)
        {
            return KPeriods == other.KPeriods && DPeriods == other.DPeriods && KSlowing == other.KSlowing && MAType == other.MAType && StochasticMode == other.StochasticMode && Equals(MarketSeries, other.MarketSeries);
        }

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is StochasticParameters && Equals((StochasticParameters) obj);
        }

        public override int GetHashCode()
        {
            unchecked
            {
                var hashCode = KPeriods;
                hashCode = (hashCode*397) ^ DPeriods;
                hashCode = (hashCode*397) ^ KSlowing;
                hashCode = (hashCode*397) ^ (int) MAType;
                hashCode = (hashCode*397) ^ (int) StochasticMode;
                hashCode = (hashCode*397) ^ (MarketSeries != null ? MarketSeries.GetHashCode() : 0);
                return hashCode;
            }
        }
    }

	private class StochasticValues_
	{
		public Mq4Double K { get; private set; }
		public Mq4Double D { get; private set; }

		public StochasticValues_(double k, double d)
		{
			K = k;
			D = d;
		}
	}

	private enum StochasticMode
	{
		LowHigh,
		CloseClose
	}

	private class StochasticIndicator_
	{
		private readonly StochasticParameters _parameters;
		private IndicatorDataSeries _fastK;
		private MovingAverage _slowK;
		private MovingAverage _averageOnSlowK;

		public StochasticIndicator_(
			StochasticParameters stochasticParameters, 
			IIndicatorsAccessor indicatorAccessor, 
			Func<IndicatorDataSeries> dataSeriesFactory)
		{
			_parameters = stochasticParameters;
			_fastK = dataSeriesFactory();
			_slowK = indicatorAccessor.MovingAverage(_fastK, _parameters.KSlowing, _parameters.MAType);
			_averageOnSlowK = indicatorAccessor.MovingAverage(_slowK.Result, _parameters.DPeriods, _parameters.MAType);
		}

		public StochasticValues_ Calculate(int index)
        {
            for (var i = index - _parameters.KSlowing * _parameters.DPeriods; i <= index; i++)
				_fastK[i] = GetFastKValue(i);	

            for (var i = Math.Max(0, index - _parameters.DPeriods); i <= index; i++)
                _slowK.Calculate(i + 1);
            var k = _slowK.Result[index];

            for (var i = Math.Max(0, index - _parameters.DPeriods); i <= index; i++)
                _averageOnSlowK.Calculate(i + 1);    
            var d = _averageOnSlowK.Result[index];

            return new StochasticValues_(k, d);
        }

		private double GetFastKValue(int index)
		{
			DataSeries low = _parameters.MarketSeries.Low;
			DataSeries high = _parameters.MarketSeries.High;
				
			if (_parameters.StochasticMode == StochasticMode.CloseClose)
			{
				low = _parameters.MarketSeries.Close;
				high = _parameters.MarketSeries.Close;
			}
			double minFromPeriod = GetMinFromPeriod(low, index, _parameters.KPeriods);
			double maxFromPeriod = GetMaxFromPeriod(high, index, _parameters.KPeriods);
			return (_parameters.MarketSeries.Close[index] - minFromPeriod) / (maxFromPeriod - minFromPeriod) * 100.0;
		}

		private static double GetMinFromPeriod(DataSeries dataSeries, int endIndex, int periods)
		{
			double num = dataSeries[endIndex];
			for (int index = endIndex; index > endIndex - periods; --index)
			{
				if (dataSeries[index] < num)
					num = dataSeries[index];
			}
			return num;
		}

		private static double GetMaxFromPeriod(DataSeries dataSeries, int endIndex, int periods)
		{
			double num = dataSeries[endIndex];
			for (int index = endIndex; index > endIndex - periods; --index)
			{
				if (dataSeries[index] > num)
					num = dataSeries[index];
			}
			return num;
		}
	}
    
    private Dictionary<StochasticParameters, StochasticIndicator_> _stochasticIndicators = new Dictionary<StochasticParameters, StochasticIndicator_>();

	private StochasticValues_ CalculateStochastic(MarketSeries marketSeries, int kPeriod, int dPeriod, int slowing, MovingAverageType maType, StochasticMode stochasticMode, int index)
	{
		var parameters = new StochasticParameters
		{
			KPeriods = kPeriod,
			DPeriods = dPeriod,
			KSlowing = slowing,
			MAType = maType,
			StochasticMode = stochasticMode,
			MarketSeries = marketSeries,
		};

		StochasticIndicator_ indicator;
		if (!_stochasticIndicators.TryGetValue(parameters, out indicator))
		{
			indicator = new StochasticIndicator_(parameters, Indicators, () => CreateDataSeries());
			_stochasticIndicators[parameters] = indicator;
		}

		return indicator.Calculate(index);
	}	
//}
[Conditional("iBearsPower")]
Mq4Double iBearsPower(Mq4String symbol, Mq4Double timeframe, Mq4Double period, Mq4Double applied_price, Mq4Double shift)
{
	var marketSeries = GetSeries(symbol, timeframe);
		
	return marketSeries.Low.Last(shift) - iMA(symbol, timeframe, period, 0, MODE_EMA, applied_price, shift);
}

[Conditional("iBullsPower")]
Mq4Double iBullsPower(Mq4String symbol, Mq4Double timeframe, Mq4Double period, Mq4Double applied_price, Mq4Double shift)
{
	var marketSeries = GetSeries(symbol, timeframe);
		
	return marketSeries.High.Last(shift) - iMA(symbol, timeframe, period, 0, MODE_EMA, applied_price, shift);
}

[Conditional("iMomentum", "iMomentumOnArray")]
//{
private Mq4Double iMomentumOnArray(Mq4DoubleArray mq4Array, Mq4Double total, Mq4Double period, Mq4Double shift) 
{
    var dataSeries = _mq4ArrayToDataSeriesConverterFactory.Create(mq4Array);

    return Indicators.MomentumOscillator(dataSeries, period).Result.Last(shift);
}

Mq4Double iMomentum(Mq4String symbol, Mq4Double timeframe, Mq4Double period, Mq4Double applied_price, Mq4Double shift)
{
	var dataSeries = ToAppliedPrice(symbol, timeframe, applied_price);

	return Indicators.MomentumOscillator(dataSeries, period).Result.Last(shift);
}
//}
[Conditional("iForce")]
//{
Mq4Double iForce(Mq4String symbol, Mq4Double timeframe, Mq4Double period, Mq4Double ma_method, Mq4Double applied_price, Mq4Double shift)
{
	var marketSeries = GetSeries(symbol, timeframe);
	return marketSeries.TickVolume.Last(shift) * 
		(iMA(symbol, timeframe, period, 0, ma_method, applied_price, shift) - iMA(symbol, timeframe, period, 0, ma_method, applied_price, shift + 1));
}
//}
[Conditional("iRVI")]
//{
Mq4Double CalculateRvi(Mq4String symbol, Mq4Double timeframe, Mq4Double period, Mq4Double shift)
{
	var marketSeries = GetSeries(symbol, timeframe);
	var close = marketSeries.Close;
	var open = marketSeries.Open;
	var high = marketSeries.High;
	var low = marketSeries.Low;
	
	var v1sum = 0d;
	var v2sum = 0d;
	for (var i = shift; i < shift + period; i++)
	{
		var v1 = ((close.Last(i + 0) - open.Last(i + 0)) + 2 * (close.Last(i + 1) - open.Last(i + 1)) + 2 * (close.Last(i + 2) - open.Last(i + 2)) + (close.Last(i + 3) - open.Last(i + 3))) / 6;
		var v2 = ((high.Last(i + 0) - low.Last(i + 0)) + 2 * (high.Last(i + 1) - low.Last(i + 1)) + 2 * (high.Last(i + 2)- low.Last(i + 2)) + (high.Last(i + 3) - low.Last(i + 3))) / 6;
		v1sum += v1;
		v2sum += v2;
	}

	return v1sum / v2sum;
}

Mq4Double iRVI(Mq4String symbol, Mq4Double timeframe, Mq4Double period, Mq4Double mode, Mq4Double shift)
{
	if (mode == MODE_MAIN)
		return CalculateRvi(symbol, timeframe, period, shift);

	return (CalculateRvi(symbol, timeframe, period, shift) + 2 * CalculateRvi(symbol, timeframe, period, shift + 1) + 2 * CalculateRvi(symbol, timeframe, period, shift + 2) +
		 CalculateRvi(symbol, timeframe, period, shift + 3)) / 6;
}
//}
[Conditional("iCustom")]
//{
internal class CustomIndicatorParameters
{
    public MarketSeries MarketSeries { get; set; }
    public object[] Parameters { get; set; }

    protected bool Equals(CustomIndicatorParameters other)
    {
        if (Parameters.Length != other.Parameters.Length)
            return false;
        for (var i = 0; i < Parameters.Length; i++)
        {
            if (!Equals(Parameters[i], other.Parameters[i]))
                return false;
        }

        return Equals(MarketSeries, other.MarketSeries);
    }

    public override bool Equals(object obj)
    {
        if (ReferenceEquals(null, obj)) return false;
        if (ReferenceEquals(this, obj)) return true;
        if (obj.GetType() != this.GetType()) return false;
        return Equals((CustomIndicatorParameters) obj);
    }

    public override int GetHashCode()
    {
        return (MarketSeries != null ? MarketSeries.GetHashCode() : 0);
    }
}

private List<DataSeries> GetAllOutputDataSeries(object indicatorInstance)
{
	var fieldInfo = indicatorInstance.GetType().GetField("AllOutputDataSeries");
    return (List<DataSeries>)fieldInfo.GetValue(indicatorInstance);
}

private object CastParameter(object parameter)
{
	if (parameter is Mq4Double)
	{
		var mq4Double = (Mq4Double)parameter;
		if (Math.Abs(mq4Double - (int)mq4Double) < Symbol.TickSize)
			return (int)mq4Double;
		return (double)mq4Double;
	}
	if (parameter is string || parameter is Mq4String)
	{
		return (string)parameter;
	}
	return parameter;
}

private object[] CastParameters<T>(object[] parameters)
{
	return parameters
		.Select(CastParameter)
		.ToArray();
}

private static object[] AddEmailParametersIfNeeded<T>(object[] parameters)
{
	var needed = typeof(T)
			.GetProperties()
			.Where(info => info.GetCustomAttributes(typeof (ParameterAttribute), false).Any())
			.Any(p => p.Name == "EmailAddressFrom");

	var result = new List<object>();
	result.AddRange(parameters);
	if (needed)	
	{
		result.Insert(0, string.Empty);
		result.Insert(0, string.Empty);
	}
	return result.ToArray();
}

private readonly Dictionary<CustomIndicatorParameters, List<DataSeries>> _customIndicatorsCache = new Dictionary<CustomIndicatorParameters, List<DataSeries>>();

Mq4Double iCustom<T>(Mq4String symbol, Mq4Double timeframe, Mq4String name, params object[] parameters) where T : Indicator
{
	var marketSeries = GetSeries(symbol, timeframe);
	var indicatorParameters = CastParameters<T>(parameters.Take(parameters.Length - 2).ToArray());	
	indicatorParameters = AddEmailParametersIfNeeded<T>(indicatorParameters);
		
	var customIndicatorParameters = new CustomIndicatorParameters{ MarketSeries = marketSeries, Parameters = indicatorParameters};
	List<DataSeries> outputSeries;
	if (!_customIndicatorsCache.TryGetValue(customIndicatorParameters, out outputSeries))
	{
		var customIndicator = Indicators.GetIndicator<T>(marketSeries, indicatorParameters);	
		outputSeries = GetAllOutputDataSeries(customIndicator);
		_customIndicatorsCache[customIndicatorParameters] = outputSeries;
	}	
		
	var mode = (int)CastParameter(parameters[parameters.Length - 2]);
	var shift = (int)CastParameter(parameters[parameters.Length - 1]);
    return outputSeries[mode].Last(shift);
}
//}
[Conditional("iAD")]
//{

private readonly Dictionary<MarketSeries, AdIndicator_> _adCache = new Dictionary<MarketSeries, AdIndicator_>();

class AdIndicator_
{
	private readonly DataSeries _volume;
	private readonly DataSeries _high;
	private readonly DataSeries _low;
	private readonly DataSeries _close;
	private readonly IndicatorDataSeries _values;

	public AdIndicator_(MarketSeries marketSeries, Func<IndicatorDataSeries> dataSeriesFactory)
	{
		_volume = marketSeries.TickVolume;
		_high = marketSeries.High;
		_low = marketSeries.Low;
		_close = marketSeries.Close;
		_values = dataSeriesFactory();
	}

	public double Calculate(int index)
	{
		if (index < 0)
			return 0;

		_values[index] = _close[index] - _low[index] - (_high[index] - _close[index]);
		if (_values[index] != 0)
		{
			if (_high[index] != _low[index])
				_values[index] = _values[index] / (_high[index] - _low[index]) * _volume[index];
			else
				_values[index] = 0;				
		}
		if (!double.IsNaN(_values[index - 1]))
			_values[index] += _values[index - 1];
		return _values[index];
	}
}
	
Mq4Double iAD(Mq4String symbol, Mq4Double timeframe, Mq4Double shift)
{
	var marketSeries = GetSeries(symbol, timeframe);
	AdIndicator_ indicator;
	if (!_adCache.TryGetValue(marketSeries, out indicator))
	{	
		indicator = new AdIndicator_(marketSeries, () => CreateDataSeries());
		_adCache[marketSeries] = indicator;
	}

	return indicator.Calculate(marketSeries.Close.InvertIndex(shift));
}
//}
[Conditional("iIchimoku")]
private Mq4Double iIchimoku(Mq4String symbol, Mq4Double timeframe, Mq4Double tenkan_sen, Mq4Double kijun_sen, Mq4Double senkou_span_b, Mq4Double mode, Mq4Double shift)
{
    var marketSeries = GetSeries(symbol, timeframe);
       
    var indicator = Indicators.IchimokuKinkoHyo(marketSeries, tenkan_sen, kijun_sen, senkou_span_b);            
    switch (mode)
    {
      case MODE_TENKANSEN:
        return indicator.TenkanSen.Last(shift);
      case MODE_KIJUNSEN:
        return indicator.KijunSen.Last(shift);
      case MODE_SENKOUSPANA:
        return indicator.SenkouSpanA.Last(shift);
      case MODE_SENKOUSPANB:
        return indicator.SenkouSpanB.Last(shift);
      case MODE_CHINKOUSPAN:
        return indicator.ChikouSpan.Last(shift);
    }
    return 0;
}

[Conditional("iAO")]
Mq4Double iAO(Mq4String symbol, Mq4Double timeframe, Mq4Double shift)
{
    var marketSeries = GetSeries(symbol, timeframe);
    return Indicators.AwesomeOscillator(marketSeries).Result.Last(shift);   
}

[Conditional("iAC")]
Mq4Double iAC(Mq4String symbol, Mq4Double timeframe, Mq4Double shift)
{
    var marketSeries = GetSeries(symbol, timeframe);
    return Indicators.AcceleratorOscillator(marketSeries).Result.Last(shift);   
}

[Conditional("iDeMarker")]
//{
class DeMarkerIndicator_
{
    private readonly IndicatorDataSeries _deMax;
    private readonly IndicatorDataSeries _deMin;
    private readonly SimpleMovingAverage _deMinSma;
    private readonly SimpleMovingAverage _deMaxSma;
    private readonly MarketSeries _marketSeries;
    public readonly IndicatorDataSeries Result;

    public DeMarkerIndicator_(MarketSeries marketSeries, int period, Func<IndicatorDataSeries> dataSeriesFactory, IIndicatorsAccessor indicatorAccessor)
    {
        _marketSeries = marketSeries;
        _deMax = dataSeriesFactory();
        _deMin = dataSeriesFactory();
        Result = dataSeriesFactory();
        _deMaxSma = indicatorAccessor.SimpleMovingAverage(_deMax, period);
        _deMinSma = indicatorAccessor.SimpleMovingAverage(_deMin, period);
    }

    public void Calculate(int index)
    {
        if (_marketSeries.High[index] > _marketSeries.High[index - 1])
            _deMax[index] = _marketSeries.High[index] - _marketSeries.High[index - 1];
        else
            _deMax[index] = 0;

        if (_marketSeries.Low[index] < _marketSeries.Low[index - 1])
            _deMin[index] = _marketSeries.Low[index - 1] - _marketSeries.Low[index];
        else
            _deMin[index] = 0;

        Result[index] = _deMaxSma.Result[index] / (_deMaxSma.Result[index] + _deMinSma.Result[index]);
    }
}

private readonly Cache<DeMarkerIndicator_> _demarkerCache = new Cache<DeMarkerIndicator_>();

Mq4Double iDeMarker(Mq4String symbol, Mq4Double timeframe, Mq4Double period, Mq4Double shift)
{
    var marketSeries = GetSeries(symbol, timeframe);
    DeMarkerIndicator_ indicator;
    if (!_demarkerCache.TryGetValue(out indicator, period, marketSeries))
    {
        indicator = new DeMarkerIndicator_(marketSeries, period, () => CreateDataSeries(), Indicators);
        _demarkerCache.Add(indicator, period, marketSeries);
    }
    return indicator.Result.Last(shift);
}
//}
[Conditional("iEnvelopes")]
//{

class EnvelopesIndicator_
{
    private readonly MovingAverage _ma;
    private readonly double _deviation;

    public EnvelopesIndicator_(DataSeries sourceSeries, int period, MovingAverageType maType, double deviation, IIndicatorsAccessor indicatorAccessor)
    {
        _ma = indicatorAccessor.MovingAverage(sourceSeries, period, maType);
        _deviation = deviation;
    }

    public double Calculate(int index, int mode)
    {
        var priceShift = _ma.Result[index] * _deviation / 100;
        if (mode == MODE_UPPER)
            return _ma.Result[index] + priceShift;
        return _ma.Result[index] - priceShift;
    }
}

private readonly Cache<EnvelopesIndicator_> _envelopesCache = new Cache<EnvelopesIndicator_>();
private EnvelopesIndicator_ GetEnvelopes(DataSeries sourceSeries, int period, MovingAverageType maType, double deviation, IIndicatorsAccessor indicatorAccessor)
{
	EnvelopesIndicator_ indicator;
	if (!_envelopesCache.TryGetValue(out indicator, sourceSeries, period, maType, deviation))
    {
        indicator = new EnvelopesIndicator_(sourceSeries, period, maType, deviation, Indicators);
        _envelopesCache.Add(indicator, sourceSeries, period, maType, deviation);
    }
	return indicator;
}

private Mq4Double iEnvelopes(Mq4String symbol, Mq4Double timeframe, Mq4Double ma_period, Mq4Double ma_method, 
    Mq4Double ma_shift, Mq4Double applied_price, Mq4Double deviation, Mq4Double mode, Mq4Double shift)
{
    var sourceSeries = ToAppliedPrice(symbol, timeframe, applied_price);
    var maType = ToMaType(ma_method);        
    
    var indicator = GetEnvelopes(sourceSeries, ma_period, maType, deviation, Indicators);
    
    var indexToCalculate = sourceSeries.InvertIndex(ma_shift + shift);
    return indicator.Calculate(indexToCalculate, mode);
}
//}
[Conditional("iMFI")]
Mq4Double iMFI(Mq4String symbol, Mq4Double timeframe, Mq4Double period, Mq4Double shift)
{
	var marketSeries = GetSeries(symbol, timeframe);
	return Indicators.MoneyFlowIndex(marketSeries, period).Result.Last(shift); 
}
[Conditional("iAlligator")]
//{
class AlligatorIndicator_
{
    private readonly MovingAverage _jawMa;
    private readonly MovingAverage _teethMa;
    private readonly MovingAverage _lipsMa;
    private readonly int _jawShift;
    private readonly int _teethShift;
    private readonly int _lipsShift;


    public AlligatorIndicator_(DataSeries sourceSeries, MovingAverageType maType, int jawPeriod, int jawShift, int teethPeriod, int teethShift, int lipsPeriod, int lipsShift, 
    	Func<IndicatorDataSeries> dataSeriesFactory, IIndicatorsAccessor indicatorAccessor)
    {
    	_lipsShift = lipsShift;
    	_teethShift = teethShift;
    	_jawShift = jawShift;
        _jawMa = indicatorAccessor.MovingAverage(sourceSeries, jawPeriod, maType);
        _teethMa = indicatorAccessor.MovingAverage(sourceSeries, teethPeriod, maType); 
        _lipsMa = indicatorAccessor.MovingAverage(sourceSeries, lipsPeriod, maType); 
    }

    public double Calculate(int mode, int index)
    {
    	switch (mode)
    	{
    		case MODE_GATORLIPS:
    			return _lipsMa.Result[index - _lipsShift];
			case MODE_GATORJAW:
    			return _jawMa.Result[index - _jawShift];
			case MODE_GATORTEETH:
    			return _teethMa.Result[index - _teethShift];
    	}	
    	return 0;
    }
}

private readonly Cache<AlligatorIndicator_> _alligatorCache = new Cache<AlligatorIndicator_>();

Mq4Double iAlligator(Mq4String symbol, Mq4Double timeframe, Mq4Double jawPeriod, Mq4Double jawShift, Mq4Double teethPeriod, Mq4Double teethShift, Mq4Double lipsPeriod, Mq4Double lipsShift, Mq4Double ma_method, Mq4Double applied_price, Mq4Double mode, Mq4Double shift)
{
    var sourceSeries = ToAppliedPrice(symbol, timeframe, applied_price);
    var maType = ToMaType(ma_method);   

    AlligatorIndicator_ indicator;
    if (!_alligatorCache.TryGetValue(out indicator, sourceSeries, maType, jawPeriod, jawShift, teethPeriod, teethShift, lipsPeriod, lipsShift))
    {
        indicator = new AlligatorIndicator_(sourceSeries, maType, jawPeriod, jawShift, teethPeriod, teethShift, lipsPeriod, lipsShift, () => CreateDataSeries(), Indicators);
        _alligatorCache.Add(indicator, sourceSeries, maType, jawPeriod, jawShift, teethPeriod, teethShift, lipsPeriod, lipsShift);
    }

    return indicator.Calculate(mode, sourceSeries.InvertIndex(shift));
}
//}