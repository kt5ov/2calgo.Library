	[Conditional("OBJ_FIBO")] 
	class Mq4Fibo : Mq4Object
	{
		private readonly TimeSeries _timeSeries;
		private readonly string _chartObjectName;

		public Mq4Fibo(string name, int type, ChartObjects chartObjects, TimeSeries timeSeries) 
			: base(name, type, chartObjects)
		{
			_timeSeries = timeSeries;
			_chartObjectName = "FIBO " + Name;
		}

		private double[] GetLevels()
		{
			var levelsCount = (int)Get(OBJPROP_FIBOLEVELS);
			var levels = new double[levelsCount];
			for (var i = 0; i < levelsCount; i++)
				levels[i] = Get(OBJPROP_FIRSTLEVEL + i);
			return levels;
		}

		private Colors LevelColor
		{
	        get
	        {                    
				int intColor = Get(OBJPROP_LEVELCOLOR);
				if (intColor != CLR_NONE)
					return Mq4Colors.GetColorByInteger(intColor);

				return Colors.Yellow;      
	        }
		}
						
		public override void Draw()
		{
			var levelStyle = Mq4LineStyles.ToLineStyle(Get(OBJPROP_LEVELSTYLE));
			var levelWidth = Get(OBJPROP_LEVELWIDTH);

			var index1 = _timeSeries.GetIndexByTime(Time1);
			var index2 = _timeSeries.GetIndexByTime(Time2);

			var extendedIndex2 = Math.Min(index1, index2) + Math.Abs(index1 - index2) * 3;
			
			var levels = GetLevels();
			for (var i = 0; i < levels.Length; i++)
			{
				var level = levels[i];
				var price = Price2 + (Price1 - Price2) * level;
				var lineName = _chartObjectName + "level" + level;

				DrawLine(lineName, Math.Min(index1, index2), price, extendedIndex2, price, LevelColor, levelWidth, levelStyle);
				var description = GetLevelDescription(i);
				DrawText(_chartObjectName + "label" + level, description, Math.Max(index1, index2), price, VerticalAlignment.Top, HorizontalAlignment.Right, LevelColor);
			}

			DrawLine(_chartObjectName + "direction line", index1, Price1, index2, Price2, Colors.Red, 1, cAlgo.API.LineStyle.Lines);
		}

		private Dictionary<int, string> _levelDescriptions = new Dictionary<int, string>();

		public void SetLevelDescription(int index, string text)
		{			
			_levelDescriptions[index] = text;
		}

		public string GetLevelDescription(int index)
		{
			string result;
			if (!_levelDescriptions.TryGetValue(index, out result))
				return (GetLevels()[index] * 100).ToString("0.#");
			
			return result;
		}
	}       