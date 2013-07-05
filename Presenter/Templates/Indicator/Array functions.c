#region Array functions

    int ArrayInitialize<T>(IMq4Array<T> array, T value)
    {
		for(var i = 0; i < array.Length; i++)
			array[i] = value;
		return array.Length;
    }

	int ArrayCopy<T>(IMq4Array<T> dest, IMq4Array<T> source, int start_dest = 0, int start_source = 0, int count = WHOLE_ARRAY)
	{
		if (count == WHOLE_ARRAY)
			count = source.Length - start_source;
		
		for (var i = 0; i < count; i++)
			dest[start_dest + i] = source[start_source + i];

		return count;
	}

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

	int ArrayResize<T>(Mq4Array<T> array, int new_size)
	{
		array.Resize(new_size);
		return new_size;
	}
		
	int ArraySize<T>(IMq4Array<T> array)
	{
		return array.Length;
	}

#endregion //Array functions