    class Mq4Arrow : Mq4Object
    {
        private readonly ChartObjects _chartObjects;
        private readonly TimeSeries _timeSeries;
		private int _index;

        public Mq4Arrow(string name, int type, ChartObjects chartObjects, TimeSeries timeSeries) 
            : base(name, type)
        {
            _chartObjects = chartObjects;
            _timeSeries = timeSeries;
        }
            
		public override void Set(int index, Mq4Double value)
		{
			base.Set(index, value);
			switch (index)        
			{ 
				case OBJPROP_TIME1:           
					var time1 = Time1;
					_index = _timeSeries.Count - 1;
					for (var i = _timeSeries.Count - 1; i >= 0; i--)
					{
						if (_timeSeries[i] < time1)
						{
							_index = i + 1;
							break;
						}
					}               
					break;
			}
		}

		private int ArrowCode
		{
			get
			{
				return Get(OBJPROP_ARROWCODE);
			}
		}

        public override void Draw()
        {
			var arrowString = ConvertedIndicator.GetArrowByCode(ArrowCode);
            _chartObjects.DrawText(Name, arrowString, _index, Price1, VerticalAlignment.Center, HorizontalAlignment.Center, Color);
        }
    }