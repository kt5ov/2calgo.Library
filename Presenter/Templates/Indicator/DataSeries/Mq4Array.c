	class Mq4Array : IMq4Array
	{
		private List<double> _data = new List<double>();
      
		private void EnsureCountIsEnough(int index)
		{
			while (_data.Count <= index)
				_data.Add(double.NaN);
		}
      
		public Mq4Double this[int index]
		{       
			get
			{
				if (index < 0)
					return double.NaN;    
            
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