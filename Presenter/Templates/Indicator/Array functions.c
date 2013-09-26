[Conditional("ArrayInitialize")]
int ArrayInitialize<T>(IMq4Array<T> array, T value)
{
	for(var i = 0; i < array.Length; i++)
		array[i] = value;
	return array.Length;
}

[Conditional("ArrayCopy")]
int ArrayCopy<T>(IMq4Array<T> dest, IMq4Array<T> source, int start_dest = 0, int start_source = 0, int count = WHOLE_ARRAY)
{
	if (count == WHOLE_ARRAY)
		count = source.Length - start_source;
		
	for (var i = 0; i < count; i++)
		dest[start_dest + i] = source[start_source + i];

	return count;
}

[Conditional("ArrayMaximum")]
int ArrayMaximum(IMq4Array<Mq4Double> array, int count = WHOLE_ARRAY, int start = 0)
{
	var result = start;
	for (var i = start + 1; i < start + count; i++)
	{
		if (array[i] > array[result])
			result = i;
	}
	return result;
}

[Conditional("ArrayMinimum")]
int ArrayMinimum(IMq4Array<Mq4Double> array, int count = WHOLE_ARRAY, int start = 0)
{
	var result = start;
	for (var i = start + 1; i < start + count; i++)
	{
		if (array[i] < array[result])
			result = i;
	}
	return result;
}

[Conditional("ArrayResize")]
int ArrayResize<T>(Mq4Array<T> array, int new_size)
{
	array.Resize(new_size);
	return new_size;
}

[Conditional("ArrayResize")]
int ArrayResize(Mq4OutputDataSeries array, int new_size)
{
	return array.Length;
}
		
[Conditional("ArraySize")]
int ArraySize<T>(IMq4Array<T> array)
{
	return array.Length;
}

[Conditional("ArraySetAsSeries")]
//{
bool ArraySetAsSeries<T>(Mq4Array<T> mq4Array, bool value)
{
	var result = mq4Array.IsInverted;
	mq4Array.IsInverted = value;
	return result;
}

bool ArraySetAsSeries(Mq4OutputDataSeries mq4OutputDataSeries, bool value)
{
	return true;
}
//}
	
[Conditional("ArrayCopySeries")]
int ArrayCopySeries(Mq4Array<Mq4Double> mq4Array, int seriesIndex, Mq4String symbol = null, int timeframe = 0)
{
	var marketSeries = GetSeries(symbol, timeframe);
	if (seriesIndex != MODE_TIME)
	{
		DataSeries dataSeries = null;
		switch (seriesIndex)
		{
			case MODE_OPEN:
				dataSeries = marketSeries.Open;
				break;
			case MODE_HIGH:
				dataSeries = marketSeries.High;
				break;
			case MODE_LOW:
				dataSeries = marketSeries.Low;
				break;
			case MODE_CLOSE:
				dataSeries = marketSeries.Close;
				break;
			case MODE_VOLUME:
				dataSeries = marketSeries.TickVolume;   
				break;
			default:
				throw new NotImplementedException("Series index " + seriesIndex + " isn't supported in ArrayCopySeries");
		}
		for (var i = 0; i < dataSeries.Count - 1; i++)
			mq4Array[i] = dataSeries.FromEnd(i);
		
		return dataSeries.Count;
	}
	else
	{
		var mq4TimeSeries = new Mq4TimeSeries(marketSeries.OpenTime);
		for (var i = 0; i < marketSeries.OpenTime.Count - 1; i++)
			mq4Array[i] = mq4TimeSeries[i];
		
		return marketSeries.OpenTime.Count;
	}
}

[Conditional("ArraySort")]
int ArraySort<T>(Mq4Array<T> mq4Array, int count = WHOLE_ARRAY, int start = 0, int sort_dir = MODE_ASCEND)
{
	mq4Array.Sort(count, start, sort_dir);
	return 0;
}

[Conditional("ArrayBsearch")]
int ArrayBsearch(Mq4Array<Mq4Double> mq4Array, double value, int count = WHOLE_ARRAY, int start = 0, int direction = MODE_ASCEND)
{
	start = Math.Max(0, start);
	if (count == WHOLE_ARRAY)
		count = mq4Array.Length - start;
	else
		count = Math.Min(mq4Array.Length - start, count);

	var end = start + count - 1;
	if (direction == MODE_ASCEND)
	{
		for (var i = end; i >= start; i--)
			if (mq4Array[i] <= value)
				return i;
	}
	if (direction == MODE_DESCEND)
	{
		for (var i = start; i <= end; i++)
			if (mq4Array[i] >= value)
				return i;
	}
	return -1;
}
