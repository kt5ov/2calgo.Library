    [Conditional("OBJ_TREND")] 
	class Mq4TrendLine : Mq4Object
    {
		private readonly TimeSeries _timeSeries;

        public Mq4TrendLine(string name, int type, ChartObjects chartObjects, TimeSeries timeSeries) 
            : base(name, type, chartObjects)
        {
			_timeSeries = timeSeries;
        }
            
        public override void Draw()
        {       
			bool isRay = Get(OBJPROP_RAY);
            if (!isRay)
			{
                DrawLine(Name, Time1, Price1, Time2, Price2, Color, Width, Mq4LineStyles.ToLineStyle(Style));
			}
			else
			{
				const int indexDistance = 1000;
				var index1 = _timeSeries.GetIndexByTime(Time1);
				var index2 = _timeSeries.GetIndexByTime(Time2);
				if (index1 < index2)
				{
					var index3 = index1 + indexDistance;
					var price3 = (-1 * index1 * Price1 + index2 * Price1 - indexDistance * Price1 + indexDistance * Price2)/(index2 - index1);

					DrawLine(Name, index1, Price1, index3, price3, Color, Width, Mq4LineStyles.ToLineStyle(Style));
				}
				if (index2 < index1)
				{
					var index3 = index2 - indexDistance;
					var price3 = (indexDistance * Price1 - index1 * Price2 + index2 * Price2 - indexDistance * Price2)/(index2 - index1);

					DrawLine(Name, index1, Price1, index3, price3, Color, Width, Mq4LineStyles.ToLineStyle(Style));
				}
			}
        }
    }