#region Array functions

    int ArrayInitialize<T>(IMq4Array<T> array, T value)
    {
      for(var i = 0; i < array.Length; i++)
        array[i] = value;
      return array.Length;
    }


#endregion //Array functions