	class Mq4StringArray : IEnumerable
	{
		private List<Mq4String> _data = new List<Mq4String>();
		private readonly Mq4String _defaultValue;
		      
		public Mq4StringArray(int size = 0)
		{
			_defaultValue = "";
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

        public void Add(Mq4String value)
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
				     
		public Mq4String this[int index]
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