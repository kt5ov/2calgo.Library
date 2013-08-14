	class Mq4ChartObjects
	{
		private readonly ChartObjects _algoChartObjects;
		private readonly TimeSeries _timeSeries;

		private readonly Dictionary<string, Mq4Object> _mq4ObjectByName = new Dictionary<string, Mq4Object>();
		private readonly List<string> _mq4ObjectNameByIndex = new List<string>();

		public Mq4ChartObjects(ChartObjects chartObjects, TimeSeries timeSeries)
		{
			_algoChartObjects = chartObjects;
			_timeSeries = timeSeries;
		}

		public int ObjectsTotal(int type)
		{
			switch (type)
			{
				[Conditional("OBJ_VLINE")] 
				//{
				case OBJ_VLINE:
					return _mq4ObjectByName.Values.OfType<Mq4VerticalLine>().Count();
				//}
				[Conditional("OBJ_HLINE")] 
				//{
				case OBJ_HLINE:
					return _mq4ObjectByName.Values.OfType<Mq4HorizontalLine>().Count();
				//}
				[Conditional("OBJ_TEXT")] 
				//{
				case OBJ_TEXT:
					return _mq4ObjectByName.Values.OfType<Mq4Text>().Count();                                     
				//}
				[Conditional("OBJ_LABEL")] 
				//{
				case OBJ_LABEL:
					return _mq4ObjectByName.Values.OfType<Mq4Label>().Count();                                     
				//}
				[Conditional("OBJ_TREND")] 
				//{
				case OBJ_TREND:
					return _mq4ObjectByName.Values.OfType<Mq4TrendLine>().Count();                                     
				//}
				[Conditional("OBJ_RECTANGLE")] 
				//{
				case OBJ_RECTANGLE:
					return _mq4ObjectByName.Values.OfType<Mq4Rectangle>().Count();                                     
				//}
				[Conditional("EMPTY")] 
				//{
				case EMPTY:
					return _mq4ObjectByName.Count;   
				//}
			}
			return 0;
		}

		public void Create(string name, int type, int window, int time1, double price1, int time2,
									double price2, int time3, double price3)
		{
			Mq4Object mq4Object = null;
			switch (type)
			{
				[Conditional("OBJ_VLINE")] 
				//{
				case OBJ_VLINE:
					mq4Object = new Mq4VerticalLine(name, type, _algoChartObjects);
					break;   
				//}
				[Conditional("OBJ_HLINE")] 
				//{
				case OBJ_HLINE:
					mq4Object = new Mq4HorizontalLine(name, type, _algoChartObjects);
					break;                        
				//}
				[Conditional("OBJ_TEXT")] 
				//{
				case OBJ_TEXT:
					mq4Object = new Mq4Text(name, type, _algoChartObjects, _timeSeries);
					break;                                        
				//}
				[Conditional("OBJ_LABEL")] 
				//{
				case OBJ_LABEL:
					mq4Object = new Mq4Label(name, type, _algoChartObjects);
					break;
				//}
				[Conditional("OBJ_TREND")] 
				//{
				case OBJ_TREND:
					mq4Object = new Mq4TrendLine(name, type, _algoChartObjects);
					break;                    
				//}
				[Conditional("OBJ_RECTANGLE")] 
				//{
				case OBJ_RECTANGLE:
					mq4Object = new Mq4Rectangle(name, type, _algoChartObjects);
					break;
				//}
			}
			if (mq4Object == null) 
				return;

			_algoChartObjects.RemoveObject(name);
			if (_mq4ObjectByName.ContainsKey(name))
			{
				_mq4ObjectByName.Remove(name);
				_mq4ObjectNameByIndex.Remove(name);
			}
			_mq4ObjectByName[name] = mq4Object;
                
			mq4Object.Set(OBJPROP_TIME1, time1);
			mq4Object.Set(OBJPROP_TIME2, time2);
			mq4Object.Set(OBJPROP_TIME3, time3);
			mq4Object.Set(OBJPROP_PRICE1, price1);
			mq4Object.Set(OBJPROP_PRICE2, price2);
			mq4Object.Set(OBJPROP_PRICE3, price3);

			mq4Object.Draw();
		}
		
		public void Set(string name, int index, Mq4Double value)
		{
			if (!_mq4ObjectByName.ContainsKey(name))
				return;
			_mq4ObjectByName[name].Set(index, value);
			_mq4ObjectByName[name].Draw();
		}

		[Conditional("ObjectGet")] 
		public Mq4Double Get(string name, int index)
		{
			if (!_mq4ObjectByName.ContainsKey(name))
				return 0;

			return _mq4ObjectByName[name].Get(index);
		}
            
		public void SetText(string name, string text, int font_size, string font, int color)
		{
			if (!_mq4ObjectByName.ContainsKey(name))
				return;
			[Conditional("OBJ_TEXT")] 
			//{
			var mq4Text = _mq4ObjectByName[name] as Mq4Text;
			if (mq4Text != null)
				mq4Text.Text = text;
			//}
			[Conditional("OBJ_LABEL")] 
			//{
			var mq4Label = _mq4ObjectByName[name] as Mq4Label;
			if (mq4Label != null)
				mq4Label.Text = text;
			//}

			Set(name, OBJPROP_COLOR, color);
		}

		[Conditional("ObjectDelete")] 
		public void Delete(string name)
		{
			_mq4ObjectByName.Remove(name);
			_mq4ObjectNameByIndex.Remove(name);
			_algoChartObjects.RemoveObject(name);
		}

		[Conditional("ObjectsDeleteAll", "ObjectDelete")] 
		public int DeleteAll(int type)
		{
			if (type == EMPTY)
			{
				var count = _mq4ObjectByName.Count;
				_algoChartObjects.RemoveAllObjects();
				_mq4ObjectByName.Clear();
				_mq4ObjectNameByIndex.Clear();
				return count;
			}
			var deletedCount = 0;
			foreach (var mq4Object in _mq4ObjectByName.Select(kvp => kvp.Value).ToArray())
			{
				if (mq4Object.Type == type)
				{
					Delete(mq4Object.Name);
					deletedCount++;
				}
			}
			return deletedCount;
		}

		[Conditional("ObjectFind")] 
		public int Find(string name)
		{
			if (_mq4ObjectByName.ContainsKey(name))
				return 0;
			return -1;
		}

		[Conditional("ObjectMove")] 
		public void Move(string name, int point, int time, double price)
		{
			if (!_mq4ObjectByName.ContainsKey(name))
				return;
			var mq4Object = _mq4ObjectByName[name];
			switch (point)
			{
				case 0:
				mq4Object.Set(OBJPROP_TIME1, time);
				mq4Object.Set(OBJPROP_PRICE1, price);
				break;
				case 1:
				mq4Object.Set(OBJPROP_TIME2, time);
				mq4Object.Set(OBJPROP_PRICE2, price);
				break;          
				case 2:
				mq4Object.Set(OBJPROP_TIME3, time);
				mq4Object.Set(OBJPROP_PRICE3, price);
				break;
			}
		}

		[Conditional("ObjectName")] 
		public string ObjectName(int index)
		{
			if (index <= 0 || index >= _mq4ObjectNameByIndex.Count)
				return string.Empty;

			return _mq4ObjectNameByIndex[index];
		}
	}
