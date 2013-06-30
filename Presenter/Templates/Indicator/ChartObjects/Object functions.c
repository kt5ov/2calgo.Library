    bool ObjectSet(string name, int index, Mq4Double value)
    {
        _mq4ChartObjects.Set(name, index, value);
		return true;
    }
                
    bool ObjectSetText(string name, string text, int font_size, string font = null, int color = CLR_NONE)
    {
		_mq4ChartObjects.SetText(name, text, font_size, font, color);
		return true;
    }

    bool ObjectCreate(string name, int type, int window, int time1, double price1, int time2 = 0, double price2 = 0, int time3 = 0, double price3 = 0)
    {
        _mq4ChartObjects.Create(name, type, window, time1, price1, time2, price2, time3, price3);
		return true;
    }
        
    bool ObjectDelete(string name)
    {
      _mq4ChartObjects.Delete(name);
      return true;
    }

    int ObjectFind(string name)
    {
      return _mq4ChartObjects.Find(name);
    }

    int ObjectsDeleteAll(int window = EMPTY, int type=EMPTY)
    {
      return _mq4ChartObjects.DeleteAll(type);
    }
	
    bool ObjectMove(string name, int point, int time, double price)
    {
      _mq4ChartObjects.Move(name, point, time, price);
      return true;
    }    

	int ObjectsTotal(int type = EMPTY)
	{
		return _mq4ChartObjects.ObjectsTotal(type);
	}

	public string ObjectName(int index)
	{
		return _mq4ChartObjects.ObjectName(index);
	}