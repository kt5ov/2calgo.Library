        abstract class Mq4Object
        {   
            protected Mq4Object(string name, int type)
            {
                Name = name;
                Type = type;
            }
      
            public int Type { get; private set; }

            public string Name { get; private set; }

            protected DateTime Time1
            {
                get
                {
                    int seconds = Get(OBJPROP_TIME1);
                    return Mq4TimeSeries.ToDateTime(seconds);
                }
            }
            
            protected double Price1
            {
                get
                {
                    return Get(OBJPROP_PRICE1);
                }
            }

            protected DateTime Time2
            {
                get
                {
                    int seconds = Get(OBJPROP_TIME2);
                    return Mq4TimeSeries.ToDateTime(seconds);
                }
            }
            
            protected double Price2
            {
                get
                {
                    return Get(OBJPROP_PRICE2);
                }
            }

            protected Colors Color
            {
                get
                {                    
					int intColor = Get(OBJPROP_COLOR);
					if (intColor != CLR_NONE)
						return Mq4Colors.GetColorByInteger(intColor);

					return Colors.Yellow;      
                }
            }

            protected int Width
            {
                get
                {
                    return Get(OBJPROP_WIDTH);
                }
            }

            protected int Style
            {
              get
              {
                return Get(OBJPROP_STYLE);
              }
            }

            public abstract void Draw();

            private readonly Dictionary<int, Mq4Double> _properties = new Dictionary<int, Mq4Double>
              {
                {OBJPROP_WIDTH, new Mq4Double(1)},
                {OBJPROP_COLOR, new Mq4Double(CLR_NONE)},
              };

            public virtual void Set(int index, Mq4Double value)
            {
                _properties[index] = value;
            }

            public Mq4Double Get(int index)
            {
                return _properties.ContainsKey(index) ? _properties[index] : new Mq4Double(0);
            }
        }