	[Conditional("OBJ_VLINE")] 
	class Mq4VerticalLine : Mq4Object
	{
		public Mq4VerticalLine(string name, int type, ChartObjects chartObjects) 
			: base(name, type, chartObjects)
		{
		}

		public override void Draw()
		{
			DrawVerticalLine(Name, Time1, Color, Width, Mq4LineStyles.ToLineStyle(Style));
		}
	}       