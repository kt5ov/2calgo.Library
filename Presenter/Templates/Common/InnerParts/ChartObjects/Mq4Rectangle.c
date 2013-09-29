	[Conditional("OBJ_RECTANGLE")] 
	class Mq4Rectangle : Mq4Object
    {
        public Mq4Rectangle(string name, int type, ChartObjects chartObjects) 
            : base(name, type, chartObjects)
        {
        }
            
        public override void Draw()
        {       
            var lineStyle = Mq4LineStyles.ToLineStyle(Style);
            DrawLine(Name + " line 1", Time1, Price1, Time2, Price1, Color, Width, lineStyle);
            DrawLine(Name + " line 2", Time2, Price1, Time2, Price2, Color, Width, lineStyle);
            DrawLine(Name + " line 3", Time2, Price2, Time1, Price2, Color, Width, lineStyle);
            DrawLine(Name + " line 4", Time1, Price2, Time1, Price1, Color, Width, lineStyle);
        }
    }