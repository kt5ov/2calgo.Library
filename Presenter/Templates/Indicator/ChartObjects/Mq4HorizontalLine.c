	[Conditional("OBJ_HLINE")]    
	class Mq4HorizontalLine : Mq4Object
    {
        private readonly ChartObjects _chartObjects;

        public Mq4HorizontalLine(string name, int type, ChartObjects chartObjects) 
            : base(name, type)
        {
            _chartObjects = chartObjects;
        }

        public override void Draw()
        {
            _chartObjects.DrawHorizontalLine(Name, Price1, Color, Width, Mq4LineStyles.ToLineStyle(Style));
        }
    }             