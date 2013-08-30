    struct Mq4Double
    {
      private readonly double _value;

      public Mq4Double(double value)
      {
        _value = value;
      }
      
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
    }   