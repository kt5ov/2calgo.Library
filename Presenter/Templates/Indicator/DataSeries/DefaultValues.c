	static class DefaultValues 
	{
		public static object GetDefaultValue<T>()
		{
			if (typeof(T) == typeof(Mq4Double))
				return new Mq4Double(0);

			return default(T);
		}
	}