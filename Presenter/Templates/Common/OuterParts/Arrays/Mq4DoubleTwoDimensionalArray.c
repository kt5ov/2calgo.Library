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

		public int Range(int index)
		{
			if (index == 0)
				return _data.Count;
			return this[0].Length;
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

		public Mq4Double this[int index1, int index2]
		{       
			get
			{
				if (index1 < 0)
					return 0;

				EnsureCountIsEnough(index1);         

				return _arrays[index1][index2];
			}    
			set
			{
				if (index1 < 0)
					return;

				EnsureCountIsEnough(index1);         

				_arrays[index1][index2] = value;
			}   
		}	
	}