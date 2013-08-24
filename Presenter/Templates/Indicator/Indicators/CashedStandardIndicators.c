	class CashedStandardIndicators
    {
        private readonly IIndicatorsAccessor _indicatorsAccessor;

        public CashedStandardIndicators(IIndicatorsAccessor indicatorsAccessor)
        {
            _indicatorsAccessor = indicatorsAccessor;
        }

		[Conditional("iMA", "iMAOnArray")]
		//{
#region iMA
        private struct MAParameters
        {
            public DataSeries Source;
            public int Periods;
            public MovingAverageType MovingAverageType;
        }
    
        private struct WellesWilderSmoothingParameters
        {
          public DataSeries Source;
          public int Periods;
        }

        private Dictionary<MAParameters, MovingAverage> _movingAverages = new Dictionary<MAParameters, MovingAverage>();
        private Dictionary<WellesWilderSmoothingParameters, WellesWilderSmoothing> _wellesWilderSmoothings = new Dictionary<WellesWilderSmoothingParameters, WellesWilderSmoothing>();

        public MovingAverage MovingAverage(DataSeries source, int periods, MovingAverageType maType)
        {
            var maParameters = new MAParameters { MovingAverageType = maType, Periods = periods, Source = source };
            if (_movingAverages.ContainsKey(maParameters))
                return _movingAverages[maParameters];

            var indicator = _indicatorsAccessor.MovingAverage(source, periods, maType);
            _movingAverages.Add(maParameters, indicator);

            return indicator;
        }
    
        public WellesWilderSmoothing WellesWilderSmoothing(DataSeries source, int periods)
        {
            var parameters = new WellesWilderSmoothingParameters { Periods = periods, Source = source };
            if (_wellesWilderSmoothings.ContainsKey(parameters))
                return _wellesWilderSmoothings[parameters];

            var indicator = _indicatorsAccessor.WellesWilderSmoothing(source, periods);
            _wellesWilderSmoothings.Add(parameters, indicator);

            return indicator;
        }
#endregion //iMA
		//}

		[Conditional("iRSI", "iRSIOnArray")]
		//{
#region iRSI
        private struct RsiParameters
        {
            public DataSeries Source;
            public int Periods;
        }
    
        private Dictionary<RsiParameters, RelativeStrengthIndex> _rsiIndicators = new Dictionary<RsiParameters, RelativeStrengthIndex>();

        public RelativeStrengthIndex RelativeStrengthIndex(DataSeries source, int periods)
        {
            var rsiParameters = new RsiParameters { Periods = periods, Source = source };
            if (_rsiIndicators.ContainsKey(rsiParameters))
                return _rsiIndicators[rsiParameters];

            var indicator = _indicatorsAccessor.RelativeStrengthIndex(source, periods);
            _rsiIndicators.Add(rsiParameters, indicator);

            return indicator;
        }
#endregion //iRSI
		//}

		[Conditional("iBands", "iBandsOnArray")]
		//{
#region iBands
        private struct BandsParameters
        {
            public DataSeries Source;
            public int Periods;
            public int Deviation;
            public MovingAverageType MovingAverageType;
        }
    
        private Dictionary<BandsParameters, BollingerBands> _bandsIndicators = new Dictionary<BandsParameters, BollingerBands>();

        public BollingerBands BollingerBands(DataSeries source, int periods, int deviation, MovingAverageType maType)
        {
            var bandsParameters = new BandsParameters { Periods = periods, Source = source, Deviation = deviation, MovingAverageType = maType };
            if (_bandsIndicators.ContainsKey(bandsParameters))
                return _bandsIndicators[bandsParameters];

            var indicator = _indicatorsAccessor.BollingerBands(source, periods, deviation, maType);
            _bandsIndicators.Add(bandsParameters, indicator);

            return indicator;
        }
#endregion //iBands
		//}

		[Conditional("iADX")]
		//{
#region iADX
        private struct AdxParameters
        {
            public int Periods;
        }
    
        private Dictionary<AdxParameters, DirectionalMovementSystem> _adxIndicators = new Dictionary<AdxParameters, DirectionalMovementSystem>();

        public DirectionalMovementSystem DirectionalMovementSystem(int periods)
        {
            var adxParameters = new AdxParameters { Periods = periods };
            if (_adxIndicators.ContainsKey(adxParameters))
                return _adxIndicators[adxParameters];

            var indicator = _indicatorsAccessor.DirectionalMovementSystem(periods);
            _adxIndicators.Add(adxParameters, indicator);

            return indicator;
        }
#endregion //iADX
		//}

		[Conditional("iATR")]
		//{
#region iATR
        private struct ATRParameters
        {
            public int Periods;
        }
    
        private Dictionary<ATRParameters, SimpleMovingAverage> _atrIndicators = new Dictionary<ATRParameters, SimpleMovingAverage>();

        public SimpleMovingAverage ATR(int periods)
        {
            var parameters = new ATRParameters { Periods = periods };
            if (_atrIndicators.ContainsKey(parameters))
                return _atrIndicators[parameters];

            var trueRange = _indicatorsAccessor.TrueRange();
            var atrIndicator = _indicatorsAccessor.SimpleMovingAverage(trueRange.Result, periods);
            _atrIndicators.Add(parameters, atrIndicator);

            return atrIndicator;
        }
#endregion //iATR
		//}

		[Conditional("iMACD")]
		//{
#region iMACD
        private struct MacdParameters
        {
            public int LongPeriod;
            public int ShortPeriod;
            public int Periods;
        }
    
        private Dictionary<MacdParameters, MacdHistogram> _macdIndicators = new Dictionary<MacdParameters, MacdHistogram>();
        
        public MacdHistogram MACD(int shortPeriod, int longPeriod, int periods)
        {
            var parameters = new MacdParameters { LongPeriod = longPeriod, ShortPeriod = shortPeriod, Periods = periods };
            if (_macdIndicators.ContainsKey(parameters))
                return _macdIndicators[parameters];

            var indicator = _indicatorsAccessor.MacdHistogram(longPeriod, shortPeriod, periods);
            _macdIndicators.Add(parameters, indicator);

            return indicator;
        }
#endregion //iMACD
		//}

		[Conditional("iCCI", "iCCIOnArray")]
		//{
#region iCCI
        private struct CciParameters
        {
            public int Periods;
        }
    
        private Dictionary<CciParameters, CommodityChannelIndex> _cciIndicators = new Dictionary<CciParameters, CommodityChannelIndex>();

        public CommodityChannelIndex CommodityChannelIndex(int periods)
        {
            var cciParameters = new CciParameters { Periods = periods };
            if (_cciIndicators.ContainsKey(cciParameters))
                return _cciIndicators[cciParameters];

            var indicator = _indicatorsAccessor.CommodityChannelIndex(periods);
            _cciIndicators.Add(cciParameters, indicator);

            return indicator;
        }
#endregion //iCCI
		//}

		[Conditional("iStdDev", "iStdDevOnArray")]
		//{
#region iStdDev
        private struct stdDevParameters
        {
            public int Periods;
            public DataSeries Source;
            public MovingAverageType MovingAverageType;
        }
    
        private Dictionary<stdDevParameters, StandardDeviation> _stdDevIndicators = new Dictionary<stdDevParameters, StandardDeviation>();

        public StandardDeviation StandardDeviation(DataSeries source, int periods, MovingAverageType movingAverageType)
        {
            var stdDevParameters = new stdDevParameters { Source = source, Periods = periods, MovingAverageType = movingAverageType };
            if (_stdDevIndicators.ContainsKey(stdDevParameters))
                return _stdDevIndicators[stdDevParameters];

            var indicator = _indicatorsAccessor.StandardDeviation(source, periods, movingAverageType);
            _stdDevIndicators.Add(stdDevParameters, indicator);

            return indicator;
        }
#endregion //iStdDev
		//}

		[Conditional("iWPR")]
		//{
#region iWPR
        private struct WprParameters
        {
            public int Periods;
        }
    
        private Dictionary<WprParameters, WilliamsPctR> _wprIndicators = new Dictionary<WprParameters, WilliamsPctR>();

        public WilliamsPctR WilliamsPctR(int periods)
        {
            var wprParameters = new WprParameters { Periods = periods };
            if (_wprIndicators.ContainsKey(wprParameters))
                return _wprIndicators[wprParameters];

            var indicator = _indicatorsAccessor.WilliamsPctR(periods);
            _wprIndicators.Add(wprParameters, indicator);

            return indicator;
        }
#endregion //iWPR
		//}

[Conditional("iSAR")]
//{
        private struct SarParameters
        {
            public double Step;
			public double Maximum;
        }
    
        private Dictionary<SarParameters, ParabolicSAR> _sarIndicators = new Dictionary<SarParameters, ParabolicSAR>();

        public ParabolicSAR ParabolicSAR(double step, double maximum)
        {
            var sarParameters = new SarParameters { Step = step, Maximum = maximum };
            if (_sarIndicators.ContainsKey(sarParameters))
                return _sarIndicators[sarParameters];

            var indicator = _indicatorsAccessor.ParabolicSAR(step, maximum);
            _sarIndicators.Add(sarParameters, indicator);

            return indicator;
        }
//}

[Conditional("iStochastic")]
//{
        private struct StochasticParameters
        {
            public int KPeriods;
			public int DPeriods;
			public int KSlowing;
			public MovingAverageType MAType;
			public StochasticMode StochasticMode;
        }

		public class StochasticValues
		{
			public Mq4Double K { get; private set; }
			public Mq4Double D { get; private set; }

			public StochasticValues(double k, double d)
			{
				K = k;
				D = d;
			}
		}

		public enum StochasticMode
		{
			LowHigh,
			CloseClose
		}

		private class Mq4StochasticIndicator
		{
			private readonly StochasticParameters _parameters;
			private IndicatorDataSeries _fastK;
			private MovingAverage _slowK;
			private MovingAverage _averageOnSlowK;
			private MarketSeries _marketSeries;

			public Mq4StochasticIndicator(StochasticParameters stochasticParameters, IIndicatorsAccessor indicatorAccessor, MarketSeries marketSeries)
			{
				_parameters = stochasticParameters;
				_marketSeries = marketSeries;
				_fastK = new IndicatorDataSeries();
				_slowK = indicatorAccessor.MovingAverage(_fastK, _parameters.KSlowing, _parameters.MAType);
				_averageOnSlowK = indicatorAccessor.MovingAverage(_slowK.Result, _parameters.DPeriods, _parameters.MAType);
			}

			public StochasticValues Calculate(int index)
			{
				for (var i = 0; i < _parameters.KSlowing * _parameters.DPeriods; i++)
                _fastK[index - i] = GetFastKValue(index - i);

				for (var i = 0; i < _parameters.DPeriods; i++)
					_slowK.Calculate(index - i);

				var k = _slowK.Result[index];
				var d = _averageOnSlowK.Result[index];

				return new StochasticValues(k, d);
			}

			private double GetFastKValue(int index)
			{
				DataSeries low = _marketSeries.Low;
				DataSeries high = _marketSeries.High;
				if (_parameters.StochasticMode == StochasticMode.CloseClose)
				{
					low = _marketSeries.Close;
					high = _marketSeries.High;
				}
				double minFromPeriod = GetMinFromPeriod(low, index, _parameters.KPeriods);
				double maxFromPeriod = GetMaxFromPeriod(high, index, _parameters.KPeriods);
				return (_marketSeries.Close[index] - minFromPeriod) / (maxFromPeriod - minFromPeriod) * 100.0;
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
    
        private Dictionary<StochasticParameters, Mq4StochasticIndicator> _stochasticIndicators = new Dictionary<StochasticParameters, Mq4StochasticIndicator>();

		public StochasticValues Stochastic(MarketSeries marketSeries, int kPeriod, int dPeriod, int slowing, MovingAverageType maType, StochasticMode stochasticMode, int index)
		{
			var parameters = new StochasticParameters
			{
				KPeriods = kPeriod,
				DPeriods = dPeriod,
				KSlowing = slowing,
				MAType = maType,
				StochasticMode = stochasticMode
			};

			Mq4StochasticIndicator indicator;
			if (!_stochasticIndicators.TryGetValue(parameters, out indicator))
			{
				indicator = new Mq4StochasticIndicator(parameters, _indicatorsAccessor, marketSeries);
				_stochasticIndicators[parameters] = indicator;
			}

			return indicator.Calculate(index);
		}		 
		       
//}
    }