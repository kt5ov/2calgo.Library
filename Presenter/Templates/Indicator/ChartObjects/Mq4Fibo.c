	[Conditional("OBJ_FIBO")] 
	class Mq4Fibo : Mq4Object
	{
		private readonly ChartObjects _chartObjects;
		private readonly TimeSeries _timeSeries;
		private readonly string _chartObjectName;

		public Mq4Fibo(string name, int type, ChartObjects chartObjects, TimeSeries timeSeries) 
			: base(name, type)
		{
			_chartObjects = chartObjects;
			_timeSeries = timeSeries;
			_chartObjectName = "FIBO " + Name;
		}
						
		public override void Draw()
		{
			var index1 = _timeSeries.GetIndexByTime(Time1);
			var index2 = _timeSeries.GetIndexByTime(Time2);

			var extendedIndex2 = Math.Min(index1, index2) + Math.Abs(index1 - index2) * 3;
			
			var levels = new [] {0, 23.6, 38.2, 50.0, 61.8, 100, 161.8, 261.8, 423.6};
			foreach (var level in levels)
			{
				var price = Price2 + (Price1 - Price2) * level / 100;
				var lineName = _chartObjectName + "level" + level;

				_chartObjects.DrawLine(lineName, Math.Min(index1, index2), price, extendedIndex2, price, Color);
				_chartObjects.DrawText(_chartObjectName + "label" + level, level.ToString("0.#"), Math.Max(index1, index2), price, VerticalAlignment.Top, HorizontalAlignment.Right, Color);
			}

			_chartObjects.DrawLine(_chartObjectName + "direction line", index1, Price1, index2, Price2, Colors.Red, 1, LineStyle.Lines);
		}
	}       