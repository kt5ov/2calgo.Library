	[Conditional("OBJ_HLINE")]    
	class Mq4HorizontalLine : Mq4Object
    {
        public Mq4HorizontalLine(string name, int type, ChartObjects chartObjects) 
            : base(name, type, chartObjects)
        {
        }

        public override void Draw()
        {
            DrawHorizontalLine(Name, Price1, Color, Width, Mq4LineStyles.ToLineStyle(Style));
        }
    }             