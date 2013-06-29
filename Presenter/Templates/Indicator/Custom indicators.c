    #region Custom indicators
    
    void IndicatorBuffers(int count) {}

    void SetIndexStyle(int index, int type, int style=EMPTY, int width=EMPTY, int clr=CLR_NONE) { }

    void IndicatorDigits(int digits) { }

    void IndicatorDigits(double digits) { }

    void SetIndexDrawBegin(int index, int begin) { }

    bool SetIndexBuffer(int index, Mq4DataSeries dataSeries) 
    {
      return true;
    }

    void IndicatorShortName(string name) { }

    void SetIndexLabel(int index, string text) { }

	const string xArrow = "\u2716";
	public Dictionary<int, string> ArrowByIndex = new Dictionary<int, string>{ {0, xArrow},  {1, xArrow},  {2, xArrow},  {3, xArrow},  {4, xArrow},  {5, xArrow},  {6, xArrow},  {7, xArrow}};
	void SetIndexArrow(int index, int code) 
	{
		ArrowByIndex[index] = GetArrowByCode(code);
	}

	string GetArrowByCode(int code)
	{
		switch(code)
		{
			case 233:
				return "\u25B2";
			case 234:
				return "\u25BC";
			default:
				return xArrow;
		}
	}

    void SetIndexShift(int index, int shift) 
    {
      _allBuffers[index].SetShift(shift);
    }
        
    void SetIndexEmptyValue(int index, double value)
    {
      _allBuffers[index].SetEmptyValue(value);
    }

	
    private int _indicatorCounted;
    private int IndicatorCounted()
    {
        return _indicatorCounted;
    }

    #endregion