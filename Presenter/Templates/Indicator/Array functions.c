#region Array functions
    
    int ArrayInitialize(Mq4Double[] array, double value)
    {
      for(var i = 0; i < array.Length; i++)
        array[i] = value;
      return array.Length;
    }

    int ArrayInitialize(Mq4DataSeries array, double value)
    {
      for(var i = 0; i < array.Count; i++)
        array[i] = value;
      return array.Count;
    }


#endregion //Array functions