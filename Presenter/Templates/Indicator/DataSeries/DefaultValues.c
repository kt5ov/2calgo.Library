	static class DefaultValues 
	{
		public static object GetDefaultValue<T>()
		{
			if (typeof(T) == typeof(Mq4Double))
				return double.NaN;

			return default(T);
		}
	}