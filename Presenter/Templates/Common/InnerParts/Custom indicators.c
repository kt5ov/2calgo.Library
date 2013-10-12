    [Conditional("IndicatorBuffers")]
	void IndicatorBuffers(int count) {}

	[Conditional("SetIndexStyle")]
    void SetIndexStyle(int index, int type, int style = EMPTY, int width = EMPTY, int clr = CLR_NONE) { }

	[Conditional("IndicatorDigits")]
    void IndicatorDigits(int digits) { }

	[Conditional("IndicatorDigits")]
    void IndicatorDigits(double digits) { }
	
	[Conditional("SetIndexDrawBegin")]
    void SetIndexDrawBegin(int index, object begin) { }

	[Conditional("SetLevelValue")]
	void SetLevelValue(int level, double value) { }

	[Conditional("SetLevelStyle")]	
	void SetLevelStyle(int drawStyle, int width, int clr = CLR_NONE) { }

	[Conditional("IndicatorShortName")]
    void IndicatorShortName(Mq4String name) { }

	[Conditional("SetIndexLabel")]
    void SetIndexLabel(int index, string text) { }

    public Dictionary<int, string> ArrowByIndex = new Dictionary<int, string>{ {0, xArrow},  {1, xArrow},  {2, xArrow},  {3, xArrow},  {4, xArrow},  {5, xArrow},  {6, xArrow},  {7, xArrow}};
    void SetIndexArrow(int index, int code) 
    {
        ArrowByIndex[index] = GetArrowByCode(code);
    }
	
	[Conditional("SetIndexShift")]
    void SetIndexShift(int index, int shift) 
    {
		AllBuffers[index].SetShift(shift);
    }
        
	[Conditional("SetIndexEmptyValue")]
    void SetIndexEmptyValue(int index, double value)
    {
		AllBuffers[index].SetEmptyValue(value);
    }
	    
    private int _indicatorCounted;
	[Conditional("IndicatorCounted")]
    private int IndicatorCounted()
    {
        return _indicatorCounted;
    }