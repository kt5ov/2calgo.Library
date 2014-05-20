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

    [Conditional("iIchimoku")]
    //{
    private Cache<IchimokuKinkoHyo> _ichimokuIndicators = new Cache<IchimokuKinkoHyo>();

    public IchimokuKinkoHyo IchimokuKinkoHyo(MarketSeries marketSeries, int tenkanSenPeriods, int kijunSenPeriods, int senkouSpanBPeriods)
    {
		IchimokuKinkoHyo indicator;
        if (_ichimokuIndicators.TryGetValue(out indicator, marketSeries, tenkanSenPeriods, kijunSenPeriods, senkouSpanBPeriods))
            return indicator;

        indicator = _indicatorsAccessor.IchimokuKinkoHyo(marketSeries, tenkanSenPeriods, kijunSenPeriods, senkouSpanBPeriods);
        _ichimokuIndicators.Add(indicator, marketSeries, tenkanSenPeriods, kijunSenPeriods, senkouSpanBPeriods);

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
	[Conditional("iMFI")]
	//{
	private Cache<MoneyFlowIndex> _mfiIndicators = new Cache<MoneyFlowIndex>();
	
	public MoneyFlowIndex MoneyFlowIndex(MarketSeries marketSeries, int period)
	{
		MoneyFlowIndex indicator;
        if (_mfiIndicators.TryGetValue(out indicator, marketSeries, period))
            return indicator;

        indicator = _indicatorsAccessor.MoneyFlowIndex(marketSeries, period);
        _mfiIndicators.Add(indicator, marketSeries, period);

        return indicator;
	}    
	//}
}