    class Mq4ChartObjects
        {
            private readonly ChartObjects _chartObjects;
            private readonly TimeSeries _timeSeries;
            private readonly Dictionary<string, Mq4Object> _createdObjects = new Dictionary<string, Mq4Object>();

            public Mq4ChartObjects(ChartObjects chartObjects, TimeSeries timeSeries)
            {
                _chartObjects = chartObjects;
                _timeSeries = timeSeries;
            }

            public void Create(string name, int type, int window, int time1, double price1, int time2,
                                     double price2, int time3, double price3)
            {
                Mq4Object mq4Object = null;
                switch (type)
                {
                    case OBJ_VLINE:
                        mq4Object = new Mq4VerticalLine(name, type, _chartObjects);
                        break;   
                    case OBJ_HLINE:
                        mq4Object = new Mq4HorizontalLine(name, type, _chartObjects);
                        break;                        
                    case OBJ_TEXT:
                        mq4Object = new Mq4Text(name, type, _chartObjects, _timeSeries);
                        break;                                        
                    case OBJ_LABEL:
                        mq4Object = new Mq4Label(name, type, _chartObjects);
                        break;
                    case OBJ_TREND:
                        mq4Object = new Mq4TrendLine(name, type, _chartObjects);
                        break;                    
                    case OBJ_RECTANGLE:
                        mq4Object = new Mq4Rectangle(name, type, _chartObjects);
                        break;
                }
                if (mq4Object == null) 
                    return;

                _chartObjects.RemoveObject(name);
                _createdObjects[name] = mq4Object;
                
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
                if (!_createdObjects.ContainsKey(name))
                    return;
                _createdObjects[name].Set(index, value);
				_createdObjects[name].Draw();
            }
            
            public void SetText(string name, string text, int font_size, string font, int color)
            {
              if (!_createdObjects.ContainsKey(name))
                    return;
                var mq4Text = _createdObjects[name] as Mq4Text;
                if (mq4Text != null)
                  mq4Text.Text = text;
                var mq4Label = _createdObjects[name] as Mq4Label;
                if (mq4Label != null)
                  mq4Label.Text = text;

                Set(name, OBJPROP_COLOR, color);
            }

      public void Delete(string name)
      {
        _createdObjects.Remove(name);
        _chartObjects.RemoveObject(name);
      }

      public int DeleteAll(int type)
      {
        if (type == EMPTY)
        {
          var count = _createdObjects.Count;
          _chartObjects.RemoveAllObjects();
          return count;
        }
        var deletedCount = 0;
        foreach (var mq4Object in _createdObjects.Select(kvp => kvp.Value).ToArray())
        {
          if (mq4Object.Type == type)
          {
            Delete(mq4Object.Name);
            deletedCount++;
          }
        }
        return deletedCount;
      }

      public int Find(string name)
      {
        if (_createdObjects.ContainsKey(name))
          return 0;
        return -1;
      }

      public void Move(string name, int point, int time, double price)
      {
        if (!_createdObjects.ContainsKey(name))
          return;
        var mq4Object = _createdObjects[name];
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
    }
