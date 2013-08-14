    [Conditional("OBJ_TEXT")] 
	class Mq4Text : Mq4Object
    {
        private readonly ChartObjects _chartObjects;
        private readonly TimeSeries _timeSeries;

        public Mq4Text(string name, int type, ChartObjects chartObjects, TimeSeries timeSeries) 
            : base(name, type)
        {
            _chartObjects = chartObjects;
            _timeSeries = timeSeries;
        }
            
		public string Text { get; set; }
            
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
            
        private int _index;

        public override void Draw()
        {       
            _chartObjects.DrawText(Name, Text, _index, Price1, VerticalAlignment.Center, HorizontalAlignment.Center, Color);
        }
    }