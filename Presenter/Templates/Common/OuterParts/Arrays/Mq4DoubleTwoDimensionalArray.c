	class Mq4DoubleTwoDimensionalArray
	{
		private List<Mq4Double> _data = new List<Mq4Double>();
		private List<Mq4DoubleArray> _arrays = new List<Mq4DoubleArray>();
		private readonly Mq4Double _defaultValue;
		private readonly int _size2;
		      
		public Mq4DoubleTwoDimensionalArray(int size2)
        {
			_defaultValue = 0;
			_size2 = size2;
		}

        public void Add(Mq4Double value)
        {
			_data.Add(value);
        }

		private void EnsureCountIsEnough(int index)
		{
			while (_arrays.Count <= index)
				_arrays.Add(new Mq4DoubleArray());
		}

		public void Initialize(Mq4Double value)
		{
			for (var i = 0; i < _data.Count; i++)
				_data[i] = value;
		}
				     
		public Mq4DoubleArray this[int index]
		{       
			get
			{
				if (index < 0)
					return new Mq4DoubleArray();

				EnsureCountIsEnough(index);         

				return _arrays[index];
			}       
		}	
	}