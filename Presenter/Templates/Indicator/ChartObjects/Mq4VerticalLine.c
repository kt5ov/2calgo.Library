	class Mq4VerticalLine : Mq4Object
	{
		private readonly ChartObjects _chartObjects;

		public Mq4VerticalLine(string name, int type, ChartObjects chartObjects) 
			: base(name, type)
		{
			_chartObjects = chartObjects;
		}

		public override void Draw()
		{
			_chartObjects.DrawVerticalLine(Name, Time1, Color, Width, Mq4LineStyles.ToLineStyle(Style));
		}
	}       