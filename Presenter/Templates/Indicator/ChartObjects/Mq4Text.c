    [Conditional("OBJ_TEXT")] 
	class Mq4Text : Mq4Object
    {
        private readonly TimeSeries _timeSeries;

        public Mq4Text(string name, int type, ChartObjects chartObjects, TimeSeries timeSeries) 
            : base(name, type, chartObjects)
        {
            _timeSeries = timeSeries;
        }
            
		public string Text { get; set; }		
            
		public override void Set(int index, Mq4Double value)
		{
			base.Set(index, value);
			switch (index)        
			{ 
				case OBJPROP_TIME1:           					
					_index = _timeSeries.GetIndexByTime(Time1);
					break;
			}
		}
            
        private int _index;

        public override void Draw()
        {       
            DrawText(Name, Text, _index, Price1, VerticalAlignment.Center, HorizontalAlignment.Center, Color);
        }
    }