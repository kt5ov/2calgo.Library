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
					_index = _timeSeries.GetIndexByTime(Time1);
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
			string arrowString;
			HorizontalAlignment horizontalAlignment;
			switch (ArrowCode)
			{
				case SYMBOL_RIGHTPRICE:
					horizontalAlignment = HorizontalAlignment.Right;
					arrowString = Price1.ToString();
					break;
				case SYMBOL_LEFTPRICE:
					horizontalAlignment = HorizontalAlignment.Left;
					arrowString = Price1.ToString();
					break;
				default:
					arrowString = ConvertedIndicator.GetArrowByCode(ArrowCode);
					horizontalAlignment = HorizontalAlignment.Center;
					break;
			}				
			_chartObjects.DrawText(Name, arrowString, _index, Price1, VerticalAlignment.Center, horizontalAlignment, Color);
        }
    }