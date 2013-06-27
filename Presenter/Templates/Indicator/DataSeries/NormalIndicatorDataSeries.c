    internal class NormalIndicatorDataSeries : IndicatorDataSeries
    {
      private List<double> _data = new List<double>();
      
      private void EnsureCountIsEnough(int index)
      {
        while (_data.Count <= index)
          _data.Add(double.NaN);
      }
      
      public double this[int index]
      {       
        get
        {
          if (index < 0)
            return double.NaN;    
            
          EnsureCountIsEnough(index);         
          
          return _data[index];
        }       
        set
        {
          if (index < 0)
            return;
            
        EnsureCountIsEnough(index);
          
          _data[index] = value;
        }
      }
      
      public int Count 
      {
        get
      {
        return _data.Count;     
      }
      }
      
      public double LastValue
      {
        get
        {
          return this[Count - 1];
        }
      }
    }