int _currentIndex;
CachedStandardIndicators _cachedStandardIndicators;
Mq4ChartObjects _mq4ChartObjects;
Mq4ArrayToDataSeriesConverterFactory _mq4ArrayToDataSeriesConverterFactory;
Mq4MarketDataSeries Open;
Mq4MarketDataSeries High;
Mq4MarketDataSeries Low;
Mq4MarketDataSeries Close;
Mq4MarketDataSeries Median;
Mq4MarketDataSeries Volume;
Mq4TimeSeries Time;

private void CommonInitialize()
{
	Open = new Mq4MarketDataSeries(MarketSeries.Open);
    High = new Mq4MarketDataSeries(MarketSeries.High);
    Low = new Mq4MarketDataSeries(MarketSeries.Low);
    Close = new Mq4MarketDataSeries(MarketSeries.Close);
    Volume = new Mq4MarketDataSeries(MarketSeries.TickVolume);
    Median = new Mq4MarketDataSeries(MarketSeries.Median);
    Time = new Mq4TimeSeries(MarketSeries.OpenTime);

    _cachedStandardIndicators = new CachedStandardIndicators(Indicators);
    _mq4ChartObjects = new Mq4ChartObjects(ChartObjects, MarketSeries.OpenTime);
	_mq4ArrayToDataSeriesConverterFactory = new Mq4ArrayToDataSeriesConverterFactory(() => CreateDataSeries());
}