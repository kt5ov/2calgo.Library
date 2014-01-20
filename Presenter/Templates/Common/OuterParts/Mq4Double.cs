struct Mq4Double : IMq4DoubleArray, IComparable, IComparable<Mq4Double>
    {
      private readonly double _value;
      private readonly Mq4DoubleArray _mq4Array;

      public Mq4Double(double value)
      {
        _value = value;
        _mq4Array = new Mq4DoubleArray();
      }

      public Mq4Double this[int index]
      {
          get { return _mq4Array[index]; }
          set { _mq4Array[index] = value; }
      }

      public int Length { get { return _mq4Array.Length; } }
      
      public static implicit operator double(Mq4Double property)
      {
        return property._value;
      }
      
      public static implicit operator int(Mq4Double property)
      {
        return (int)property._value;
      }     

      public static implicit operator bool(Mq4Double property)
      {
        return (int)property._value != 0;
      }

      public static implicit operator Mq4Double(double value)
      {
        return new Mq4Double(value);
      }

      public static implicit operator Mq4Double(int value)
      {
        return new Mq4Double(value);
      }

      public static implicit operator Mq4Double(bool value)
      {
        return new Mq4Double(value ? 1 : 0);
      }
      
      public static Mq4Double operator +(Mq4Double d1, Mq4Double d2) 
      {
        return new Mq4Double(d1._value + d2._value);
      }
      
      public static Mq4Double operator -(Mq4Double d1, Mq4Double d2) 
      {
        return new Mq4Double(d1._value - d2._value);
      }

      public static Mq4Double operator -(Mq4Double d) 
      {
        return new Mq4Double(-d._value);
      }
      
      public static Mq4Double operator +(Mq4Double d) 
      {
        return new Mq4Double(+d._value);
      }
                  
      public static Mq4Double operator *(Mq4Double d1, Mq4Double d2) 
      {
        return new Mq4Double(d1._value * d2._value);
      }
      
      public static Mq4Double operator /(Mq4Double d1, Mq4Double d2) 
      {
        return new Mq4Double(d1._value / d2._value);
      }
      
      public static bool operator ==(Mq4Double d1, Mq4Double d2) 
      {
        return d1._value == d2._value;
      }
            
      public static bool operator >(Mq4Double d1, Mq4Double d2) 
      {
        return d1._value > d2._value;
      } 
      
      public static bool operator >=(Mq4Double d1, Mq4Double d2) 
      {
        return d1._value >= d2._value;
      }
            
      public static bool operator <(Mq4Double d1, Mq4Double d2) 
      {
        return d1._value < d2._value;
      }
            
      public static bool operator <=(Mq4Double d1, Mq4Double d2) 
      {
        return d1._value <= d2._value;
      }
      
      public static bool operator !=(Mq4Double d1, Mq4Double d2) 
      {
		return d1._value != d2._value;
      }
          
      public override string ToString()
      {
		return _value.ToString();
      }

      public int CompareTo(object obj)
      {
          return _value.CompareTo(obj);
      }

      public int CompareTo(Mq4Double obj)
      {
          return _value.CompareTo(obj);
      }
}   