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

    void SetIndexArrow(int index, int code) { }

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