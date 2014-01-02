	class CachedStandardIndicators
    {
        private readonly IIndicatorsAccessor _indicatorsAccessor;

        public CachedStandardIndicators(IIndicatorsAccessor indicatorsAccessor)
        {
            _indicatorsAccessor = indicatorsAccessor;
        }

		[Conditional("iMA", "iMAOnArray", "iBearsPower", "iBullsPower", "iForce")]
		//{
#region iMA

        private Cache<MovingAverage> _movingAverages = new Cache<MovingAverage>();
        private Cache<WellesWilderSmoothing> _wellesWilderSmoothings = new Cache<WellesWilderSmoothing>();

        public MovingAverage MovingAverage(DataSeries source, int periods, MovingAverageType maType)
        {
			MovingAverage indicator;

            if (_movingAverages.TryGetValue(out indicator, maType, periods, source))
                return indicator;

            indicator = _indicatorsAccessor.MovingAverage(source, periods, maType);
            _movingAverages.Add(indicator, maType, periods, source);

            return indicator;
        }
    
        public WellesWilderSmoothing WellesWilderSmoothing(DataSeries source, int periods)
        {
			WellesWilderSmoothing indicator;

            if (_wellesWilderSmoothings.TryGetValue(out indicator, periods, source))
                return indicator;

            indicator = _indicatorsAccessor.WellesWilderSmoothing(source, periods);
            _wellesWilderSmoothings.Add(indicator, periods, source);

            return indicator;
        }
#endregion //iMA
		//}

		[Conditional("iRSI", "iRSIOnArray")]
		//{
#region iRSI
    
        private Cache<RelativeStrengthIndex> _rsiIndicators = new Cache<RelativeStrengthIndex>();

        public RelativeStrengthIndex RelativeStrengthIndex(DataSeries source, int periods)
        {
			RelativeStrengthIndex indicator;
            if (_rsiIndicators.TryGetValue(out indicator, periods, source))
                return indicator;

            indicator = _indicatorsAccessor.RelativeStrengthIndex(source, periods);
            _rsiIndicators.Add(indicator, periods, source);

            return indicator;
        }
#endregion //iRSI
		//}

		[Conditional("iBands", "iBandsOnArray")]
		//{
#region iBands    
        private Cache<BollingerBands> _bandsIndicators = new Cache<BollingerBands>();

        public BollingerBands BollingerBands(DataSeries source, int periods, int deviation, MovingAverageType maType)
        {
			BollingerBands indicator;

            if (_bandsIndicators.TryGetValue(out indicator, periods, source, deviation, maType))
                return indicator;

            indicator = _indicatorsAccessor.BollingerBands(source, periods, deviation, maType);
            _bandsIndicators.Add(indicator, periods, source, deviation, maType);

            return indicator;
        }
#endregion //iBands
		//}

		[Conditional("iADX")]
		//{
#region iADX
    
        private Cache<DirectionalMovementSystem> _adxIndicators = new Cache<DirectionalMovementSystem>();

        public DirectionalMovementSystem DirectionalMovementSystem(MarketSeries marketSeries, int periods)
        {
			DirectionalMovementSystem indicator;
            if (_adxIndicators.TryGetValue(out indicator, periods, marketSeries))
                return indicator;

            indicator = _indicatorsAccessor.DirectionalMovementSystem(marketSeries, periods);
            _adxIndicators.Add(indicator, periods, marketSeries);

            return indicator;
        }
#endregion //iADX
		//}

		[Conditional("iATR")]
		//{
#region iATR
    
        private Cache<SimpleMovingAverage> _atrIndicators = new Cache<SimpleMovingAverage>();

        public SimpleMovingAverage ATR(MarketSeries series, int periods)
        {
			SimpleMovingAverage indicator;
            if (_atrIndicators.TryGetValue(out indicator, periods, series))
                return indicator;

            var trueRange = _indicatorsAccessor.TrueRange(series);
            indicator = _indicatorsAccessor.SimpleMovingAverage(trueRange.Result, periods);
            _atrIndicators.Add(indicator, periods, series);

            return indicator;
        }
#endregion //iATR
		//}

		[Conditional("iMACD", "iOsMA")]
		//{
#region iMACD
    
        private Cache<MacdCrossOver> _macdIndicators = new Cache<MacdCrossOver>();
        
        public MacdCrossOver MacdCrossOver(DataSeries series, int shortPeriod, int longPeriod, int periods)
        {
			MacdCrossOver indicator;
            if (_macdIndicators.TryGetValue(out indicator, series, shortPeriod, longPeriod, periods))
                return indicator;

            indicator = _indicatorsAccessor.MacdCrossOver(series, longPeriod, shortPeriod, periods);
            _macdIndicators.Add(indicator, series, shortPeriod, longPeriod, periods);

            return indicator;
        }
#endregion //iMACD
		//}

		[Conditional("iCCI", "iCCIOnArray")]
		//{
#region iCCI
    
        private Dictionary<int, CommodityChannelIndex> _cciIndicators = new Dictionary<int, CommodityChannelIndex>();

        public CommodityChannelIndex CommodityChannelIndex(int periods)
        {
            if (_cciIndicators.ContainsKey(periods))
                return _cciIndicators[periods];

            var indicator = _indicatorsAccessor.CommodityChannelIndex(periods);
            _cciIndicators.Add(periods, indicator);

            return indicator;
        }
#endregion //iCCI
		//}

		[Conditional("iStdDev", "iStdDevOnArray")]
		//{
#region iStdDev
    
        private Cache<StandardDeviation> _stdDevIndicators = new Cache<StandardDeviation>();

        public StandardDeviation StandardDeviation(DataSeries source, int periods, MovingAverageType movingAverageType)
        {
			StandardDeviation indicator;
            if (_stdDevIndicators.TryGetValue(out indicator, source, periods, movingAverageType))
                return indicator;

            indicator = _indicatorsAccessor.StandardDeviation(source, periods, movingAverageType);
            _stdDevIndicators.Add(indicator, source, periods, movingAverageType);

            return indicator;
        }
#endregion //iStdDev
		//}

		[Conditional("iWPR")]
		//{
#region iWPR
        private Cache<WilliamsPctR> _wprIndicators = new Cache<WilliamsPctR>();

        public WilliamsPctR WilliamsPctR(MarketSeries marketSeries, int periods)
        {
			WilliamsPctR indicator;
            if (_wprIndicators.TryGetValue(out indicator, marketSeries, periods))
                return indicator;

            indicator = _indicatorsAccessor.WilliamsPctR(marketSeries, periods);
            _wprIndicators.Add(indicator, marketSeries, periods);

            return indicator;
        }
#endregion //iWPR
		//}

[Conditional("iSAR")]
//{
    private Cache<ParabolicSAR> _sarIndicators = new Cache<ParabolicSAR>();

    public ParabolicSAR ParabolicSAR(MarketSeries series, double step, double maximum)
    {
		ParabolicSAR indicator;
        if (_sarIndicators.TryGetValue(out indicator, series, step, maximum))
            return indicator;

        indicator = _indicatorsAccessor.ParabolicSAR(series, step, maximum);
        _sarIndicators.Add(indicator, series, step, maximum);

        return indicator;
    }
//}
    [Conditional("iMomentum")]
    //{
    private struct MomentumParameters
    {
    	public DataSeries DataSeries;
    	public int Period;
    }

    private Dictionary<MomentumParameters, MomentumOscillator> _momentumIndicators = new Dictionary<MomentumParameters, MomentumOscillator>();

    public MomentumOscillator MomentumOscillator(DataSeries dataSeries, int period)
    {
        var parameters = new MomentumParameters { DataSeries = dataSeries, Period = period };
        if (_momentumIndicators.ContainsKey(parameters))
            return _momentumIndicators[parameters];

        var indicator = _indicatorsAccessor.MomentumOscillator(dataSeries, period);
        _momentumIndicators.Add(parameters, indicator);

        return indicator;
    }
    //}
    [Conditional("iIchimoku")]
    //{
    struct IchimokuParameters
    {
        public MarketSeries MarketSeries;
        public int TenkanSenPeriods;
        public int KijunSenPeriods;
        public int SenkouSpanBPeriods;
    }

    private Dictionary<IchimokuParameters, IchimokuKinkoHyo> _ichimokuIndicators = new Dictionary<IchimokuParameters, IchimokuKinkoHyo>();

    public IchimokuKinkoHyo IchimokuKinkoHyo(MarketSeries marketSeries, int tenkanSenPeriods, int kijunSenPeriods, int senkouSpanBPeriods)
    {
        var parameters = new IchimokuParameters { MarketSeries = marketSeries, TenkanSenPeriods = tenkanSenPeriods, KijunSenPeriods = kijunSenPeriods, SenkouSpanBPeriods = senkouSpanBPeriods };
        if (_ichimokuIndicators.ContainsKey(parameters))
            return _ichimokuIndicators[parameters];

        var indicator = _indicatorsAccessor.IchimokuKinkoHyo(marketSeries, tenkanSenPeriods, kijunSenPeriods, senkouSpanBPeriods);
        _ichimokuIndicators.Add(parameters, indicator);

        return indicator;
    }
    //}
	
	[Conditional("iAO")]
	//{
	private readonly Dictionary<MarketSeries, AwesomeOscillator> _aoCache = new Dictionary<MarketSeries, AwesomeOscillator>();

	public AwesomeOscillator AwesomeOscillator(MarketSeries marketSeries)
	{
		if (_aoCache.ContainsKey(marketSeries))
			return _aoCache[marketSeries];

		var indicator = _indicatorsAccessor.AwesomeOscillator(marketSeries);
		_aoCache.Add(marketSeries, indicator);
		return indicator;
	}    
	//}
	[Conditional("iAC")]
	//{
	private readonly Dictionary<MarketSeries, AcceleratorOscillator> _acCache = new Dictionary<MarketSeries, AcceleratorOscillator>();

	public AcceleratorOscillator AcceleratorOscillator(MarketSeries marketSeries)
	{
		if (_acCache.ContainsKey(marketSeries))
			return _acCache[marketSeries];

		var indicator = _indicatorsAccessor.AcceleratorOscillator(marketSeries);
		_acCache.Add(marketSeries, indicator);
		return indicator;
	}    
	//}
}