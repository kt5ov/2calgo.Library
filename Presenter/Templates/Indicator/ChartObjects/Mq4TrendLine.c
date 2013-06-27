    class Mq4TrendLine : Mq4Object
    {
        private readonly ChartObjects _chartObjects;

        public Mq4TrendLine(string name, int type, ChartObjects chartObjects) 
            : base(name, type)
        {
            _chartObjects = chartObjects;
        }
            
        public override void Draw()
        {       
            _chartObjects.DrawLine(Name, Time1, Price1, Time2, Price2, Color, Width, Mq4LineStyles.ToLineStyle(Style));
        }
    }