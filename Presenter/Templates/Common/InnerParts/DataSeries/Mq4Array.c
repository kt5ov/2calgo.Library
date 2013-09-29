	class Mq4Array<T> : IMq4Array<T>,  IEnumerable
	{
		private List<T> _data = new List<T>();
		private readonly T _defaultValue;
		      
		public Mq4Array(int size = 0)
		{
			_defaultValue = (T)DefaultValues.GetDefaultValue<T>();
		}

		public IEnumerator GetEnumerator()
		{
            return _data.GetEnumerator();
        }

		private bool _isInverted;
		public bool IsInverted
		{
			get { return _isInverted; }
			set { _isInverted = value; }
		}

        public void Add(T value)
        {
			_data.Add(value);
        }

		private void EnsureCountIsEnough(int index)
		{
			while (_data.Count <= index)
				_data.Add(_defaultValue);
		}

		public int Length 
		{
			get { return _data.Count; }
		}

		public void Resize(int newSize)
		{ 
			while (newSize < _data.Count)
				_data.RemoveAt(_data.Count - 1);

			while (newSize > _data.Count)
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
				Changed.Raise(index, value);
			}
		}	

		[Conditional("ArraySort")]
		//{
		public void Sort(int count, int start, int sort_dir)
		{
			start = Math.Max(0, start);
			if (count == WHOLE_ARRAY)
				count = _data.Count - start;
			else
				count = Math.Min(_data.Count - start, count);
			
			var comparer = Comparers.GetComparer<T>();
			_data.Sort(start, count, comparer);
			if (sort_dir == MODE_DESCEND)
			{				
				for (var i = start; i < start + count / 2; i++)
				{
					var x = _data[i];
					_data[i] = _data[start + count - 1 - i];
					_data[start + count - 1 - i] = x;
				}
			}
		}
		//}

		public event Action<int, T> Changed;
	}