	class Mq4Array<T> : IMq4Array<T>
	{
		private List<T> _data = new List<T>();
		private readonly T _defaultValue;
      
		public Mq4Array(params T[] values)
		{
			_defaultValue = (T)DefaultValues.GetDefaultValue<T>();
			for (var i = 0; i < values.Length; i++)
				this[i] = values[i];
		}

		private void EnsureCountIsEnough(int index)
		{
			while (_data.Count <= index)
				_data.Add(_defaultValue);
		}
				     
		public T this[int index]
		{       
			get
			{
				if (index < 0)
					return _defaultValue;    
            
				EnsureCountIsEnough(index);         
          
				return _data[index];
			}       
			set
			{
				if (index < 0)
					return;
            
				EnsureCountIsEnough(index);
          
				_data[index] = value;
			}
		}
	}