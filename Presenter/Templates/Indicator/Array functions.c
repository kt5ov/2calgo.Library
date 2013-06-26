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
    
    class Mq4ChartObjects
        {
            private readonly ChartObjects _chartObjects;
            private readonly TimeSeries _timeSeries;
            private readonly Dictionary<string, Mq4Object> _createdObjects = new Dictionary<string, Mq4Object>();

            public Mq4ChartObjects(ChartObjects chartObjects, TimeSeries timeSeries)
            {
                _chartObjects = chartObjects;
                _timeSeries = timeSeries;
            }

            public void Create(string name, int type, int window, int time1, double price1, int time2,
                                     double price2, int time3, double price3)
            {
                Mq4Object mq4Object = null;
                switch (type)
                {
                    case OBJ_VLINE:
                        mq4Object = new Mq4VerticalLine(name, type, _chartObjects);
                        break;   
                    case OBJ_HLINE:
                        mq4Object = new Mq4HorizontalLine(name, type, _chartObjects);
                        break;                        
                    case OBJ_TEXT:
                        mq4Object = new Mq4Text(name, type, _chartObjects, _timeSeries);
                        break;                                        
                    case OBJ_LABEL:
                        mq4Object = new Mq4Label(name, type, _chartObjects);
                        break;
                    case OBJ_TREND:
                        mq4Object = new Mq4TrendLine(name, type, _chartObjects);
                        break;                    
                    case OBJ_RECTANGLE:
                        mq4Object = new Mq4Rectangle(name, type, _chartObjects);
                        break;
                }
                if (mq4Object == null) 
                    return;

                _chartObjects.RemoveObject(name);
                _createdObjects[name] = mq4Object;
                
                mq4Object.Set(OBJPROP_TIME1, time1);
                mq4Object.Set(OBJPROP_TIME2, time2);
                mq4Object.Set(OBJPROP_TIME3, time3);
                mq4Object.Set(OBJPROP_PRICE1, price1);
                mq4Object.Set(OBJPROP_PRICE2, price2);
                mq4Object.Set(OBJPROP_PRICE3, price3);

                mq4Object.Draw();
            }

            public void Set(string name, int index, Mq4Double value)
            {
                if (!_createdObjects.ContainsKey(name))
                    return;
                _createdObjects[name].Set(index, value);
        _createdObjects[name].Draw();
            }
            
            public void SetText(string name, string text, int font_size, string font, int color)
            {
              if (!_createdObjects.ContainsKey(name))
                    return;
                var mq4Text = _createdObjects[name] as Mq4Text;
                if (mq4Text != null)
                  mq4Text.Text = text;
                var mq4Label = _createdObjects[name] as Mq4Label;
                if (mq4Label != null)
                  mq4Label.Text = text;

                Set(name, OBJPROP_COLOR, color);
            }

      public void Delete(string name)
      {
        _createdObjects.Remove(name);
        _chartObjects.RemoveObject(name);
      }

      public int DeleteAll(int type)
      {
        if (type == EMPTY)
        {
          var count = _createdObjects.Count;
          _chartObjects.RemoveAllObjects();
          return count;
        }
        var deletedCount = 0;
        foreach (var mq4Object in _createdObjects.Select(kvp => kvp.Value).ToArray())
        {
          if (mq4Object.Type == type)
          {
            Delete(mq4Object.Name);
            deletedCount++;
          }
        }
        return deletedCount;
      }

      public int Find(string name)
      {
        if (_createdObjects.ContainsKey(name))
          return 0;
        return -1;
      }

      public void Move(string name, int point, int time, double price)
      {
        if (!_createdObjects.ContainsKey(name))
          return;
        var mq4Object = _createdObjects[name];
        switch (point)
        {
          case 0:
            mq4Object.Set(OBJPROP_TIME1, time);
            mq4Object.Set(OBJPROP_PRICE1, price);
            break;
          case 1:
            mq4Object.Set(OBJPROP_TIME2, time);
            mq4Object.Set(OBJPROP_PRICE2, price);
            break;          
          case 2:
            mq4Object.Set(OBJPROP_TIME3, time);
            mq4Object.Set(OBJPROP_PRICE3, price);
            break;
        }
      }
        }

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

                return Colors.Blue;      
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

        class Mq4VerticalLine : Mq4Object
        {
            private readonly ChartObjects _chartObjects;

            public Mq4VerticalLine(string name, int type, ChartObjects chartObjects) 
                : base(name, type)
            {
                _chartObjects = chartObjects;
            }

            public override void Draw()
            {
              _chartObjects.DrawVerticalLine(Name, Time1, Color, Width, Mq4LineStyles.ToLineStyle(Style));
            }
        }       

        class Mq4HorizontalLine : Mq4Object
        {
            private readonly ChartObjects _chartObjects;

            public Mq4HorizontalLine(string name, int type, ChartObjects chartObjects) 
                : base(name, type)
            {
                _chartObjects = chartObjects;
            }

            public override void Draw()
            {
              _chartObjects.DrawHorizontalLine(Name, Price1, Color, Width, Mq4LineStyles.ToLineStyle(Style));
            }
        }     
        
        class Mq4Text : Mq4Object
        {
            private readonly ChartObjects _chartObjects;
            private readonly TimeSeries _timeSeries;

            public Mq4Text(string name, int type, ChartObjects chartObjects, TimeSeries timeSeries) 
                : base(name, type)
            {
                _chartObjects = chartObjects;
                _timeSeries = timeSeries;
            }
            
            public string Text { get; set; }
            
      public override void Set(int index, Mq4Double value)
      {
        base.Set(index, value);
        switch (index)        
        { 
          case OBJPROP_TIME1:           
            var time1 = Time1;
            _index = _timeSeries.Count - 1;
                  for (var i = _timeSeries.Count - 1; i >= 0; i--)
                  {
                    if (_timeSeries[i] < time1)
                    {
                      _index = i + 1;
                      break;
                    }
                  }               
                  break;
        }
      }
            
            private int _index;

            public override void Draw()
            {       
                _chartObjects.DrawText(Name, Text, _index, Price1, VerticalAlignment.Center, HorizontalAlignment.Center, Color);
            }
        }

        class Mq4Label : Mq4Object
        {
            private readonly ChartObjects _chartObjects;

            public Mq4Label(string name, int type, ChartObjects chartObjects) 
                : base(name, type)
            {
                _chartObjects = chartObjects;
            }

            public string Text { get; set; }

            private int X 
            {
              get
              {
                return Get(OBJPROP_XDISTANCE);
              }
            }

            private int Corner
            {
              get
              {
                return Get(OBJPROP_CORNER);
              }
            }

            private int Y
            {
              get
              {
                return Get(OBJPROP_YDISTANCE);
              }
            }

            private string MultiplyString(string str, int count)
          {
            var stringBuilder = new StringBuilder();
            for (var i = 0; i < count; i++)         
              stringBuilder.Append(str);
        return stringBuilder.ToString();            
          }    

      private string GetSpaces(int count)
          {
            return MultiplyString(" ", count);
          }
          
      private string GetNewLines(int count)
          {
            return MultiplyString("\r\n", count);
          }

            public override void Draw()
            {                               
                const double spaceWidth = 5.4;
        const double lineHeight = 20;

        var xSpaces = GetSpaces((int)Math.Ceiling(X / spaceWidth));
        var ySpaces = GetNewLines((int)Math.Ceiling(Y / lineHeight));
        
        switch(Corner)
        {
          case 0:
                _chartObjects.DrawText(Name, ySpaces + xSpaces + Text, StaticPosition.TopLeft, Color);
            break;
          case 1:
                _chartObjects.DrawText(Name, ySpaces + Text + xSpaces + '.', StaticPosition.TopRight, Color);
            break;          
          case 2:
                _chartObjects.DrawText(Name, xSpaces + Text + ySpaces, StaticPosition.BottomLeft, Color);
            break;          
          case 3:
                _chartObjects.DrawText(Name, Text + xSpaces + '.' + ySpaces, StaticPosition.BottomRight, Color);
            break;  
        }
            }
        }

        class Mq4TrendLine : Mq4Object
        {
            private readonly ChartObjects _chartObjects;

            public Mq4TrendLine(string name, int type, ChartObjects chartObjects) 
                : base(name, type)
            {
                _chartObjects = chartObjects;
            }
            
            public override void Draw()
            {       
                _chartObjects.DrawLine(Name, Time1, Price1, Time2, Price2, Color, Width, Mq4LineStyles.ToLineStyle(Style));
            }
        }

        class Mq4Rectangle : Mq4Object
        {
            private readonly ChartObjects _chartObjects;

            public Mq4Rectangle(string name, int type, ChartObjects chartObjects) 
                : base(name, type)
            {
                _chartObjects = chartObjects;
            }
            
            public override void Draw()
            {       
              var lineStyle = Mq4LineStyles.ToLineStyle(Style);
                _chartObjects.DrawLine(Name + " line 1", Time1, Price1, Time2, Price1, Color, Width, lineStyle);
                _chartObjects.DrawLine(Name + " line 2", Time2, Price1, Time2, Price2, Color, Width, lineStyle);
                _chartObjects.DrawLine(Name + " line 3", Time2, Price2, Time1, Price2, Color, Width, lineStyle);
                _chartObjects.DrawLine(Name + " line 4", Time1, Price2, Time1, Price1, Color, Width, lineStyle);
            }
        }

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
    }

    class CashedStandardIndicators
    {
        private readonly IIndicatorsAccessor _indicatorsAccessor;

        public CashedStandardIndicators(IIndicatorsAccessor indicatorsAccessor)
        {
            _indicatorsAccessor = indicatorsAccessor;
        }

#region iMA
        private struct MAParameters
        {
            public DataSeries Source;
            public int Periods;
            public MovingAverageType MovingAverageType;
        }
    
        private struct WellesWilderSmoothingParameters
        {
          public DataSeries Source;
          public int Periods;
        }

        private Dictionary<MAParameters, MovingAverage> _movingAverages = new Dictionary<MAParameters, MovingAverage>();
        private Dictionary<WellesWilderSmoothingParameters, WellesWilderSmoothing> _wellesWilderSmoothings = new Dictionary<WellesWilderSmoothingParameters, WellesWilderSmoothing>();

        public MovingAverage MovingAverage(DataSeries source, int periods, MovingAverageType maType)
        {
            var maParameters = new MAParameters { MovingAverageType = maType, Periods = periods, Source = source };
            if (_movingAverages.ContainsKey(maParameters))
                return _movingAverages[maParameters];

            var indicator = _indicatorsAccessor.MovingAverage(source, periods, maType);
            _movingAverages.Add(maParameters, indicator);

            return indicator;
        }
    
        public WellesWilderSmoothing WellesWilderSmoothing(DataSeries source, int periods)
        {
            var parameters = new WellesWilderSmoothingParameters { Periods = periods, Source = source };
            if (_wellesWilderSmoothings.ContainsKey(parameters))
                return _wellesWilderSmoothings[parameters];

            var indicator = _indicatorsAccessor.WellesWilderSmoothing(source, periods);
            _wellesWilderSmoothings.Add(parameters, indicator);

            return indicator;
        }
#endregion //iMA

#region iRSI
        private struct RsiParameters
        {
            public DataSeries Source;
            public int Periods;
        }
    
        private Dictionary<RsiParameters, RelativeStrengthIndex> _rsiIndicators = new Dictionary<RsiParameters, RelativeStrengthIndex>();

        public RelativeStrengthIndex RelativeStrengthIndex(DataSeries source, int periods)
        {
            var rsiParameters = new RsiParameters { Periods = periods, Source = source };
            if (_rsiIndicators.ContainsKey(rsiParameters))
                return _rsiIndicators[rsiParameters];

            var indicator = _indicatorsAccessor.RelativeStrengthIndex(source, periods);
            _rsiIndicators.Add(rsiParameters, indicator);

            return indicator;
        }
#endregion //iRSI

#region iBands
        private struct BandsParameters
        {
            public DataSeries Source;
            public int Periods;
            public int Deviation;
            public MovingAverageType MovingAverageType;
        }
    
        private Dictionary<BandsParameters, BollingerBands> _bandsIndicators = new Dictionary<BandsParameters, BollingerBands>();

        public BollingerBands BollingerBands(DataSeries source, int periods, int deviation, MovingAverageType maType)
        {
            var bandsParameters = new BandsParameters { Periods = periods, Source = source, Deviation = deviation, MovingAverageType = maType };
            if (_bandsIndicators.ContainsKey(bandsParameters))
                return _bandsIndicators[bandsParameters];

            var indicator = _indicatorsAccessor.BollingerBands(source, periods, deviation, maType);
            _bandsIndicators.Add(bandsParameters, indicator);

            return indicator;
        }
#endregion //iBands

#region iADX
        private struct AdxParameters
        {
            public int Periods;
        }
    
        private Dictionary<AdxParameters, DirectionalMovementSystem> _adxIndicators = new Dictionary<AdxParameters, DirectionalMovementSystem>();

        public DirectionalMovementSystem DirectionalMovementSystem(int periods)
        {
            var adxParameters = new AdxParameters { Periods = periods };
            if (_adxIndicators.ContainsKey(adxParameters))
                return _adxIndicators[adxParameters];

            var indicator = _indicatorsAccessor.DirectionalMovementSystem(periods);
            _adxIndicators.Add(adxParameters, indicator);

            return indicator;
        }
#endregion //iADX

#region iATR
        private struct ATRParameters
        {
            public int Periods;
        }
    
        private Dictionary<ATRParameters, SimpleMovingAverage> _atrIndicators = new Dictionary<ATRParameters, SimpleMovingAverage>();

        public SimpleMovingAverage ATR(int periods)
        {
            var parameters = new ATRParameters { Periods = periods };
            if (_atrIndicators.ContainsKey(parameters))
                return _atrIndicators[parameters];

            var trueRange = _indicatorsAccessor.TrueRange();
            var atrIndicator = _indicatorsAccessor.SimpleMovingAverage(trueRange.Result, periods);
            _atrIndicators.Add(parameters, atrIndicator);

            return atrIndicator;
        }
#endregion //iATR

#region iMACD
        private struct MacdParameters
        {
            public int LongPeriod;
            public int ShortPeriod;
            public int Periods;
        }
    
        private Dictionary<MacdParameters, MacdHistogram> _macdIndicators = new Dictionary<MacdParameters, MacdHistogram>();
        
        public MacdHistogram MACD(int shortPeriod, int longPeriod, int periods)
        {
            var parameters = new MacdParameters { LongPeriod = longPeriod, ShortPeriod = shortPeriod, Periods = periods };
            if (_macdIndicators.ContainsKey(parameters))
                return _macdIndicators[parameters];

            var indicator = _indicatorsAccessor.MacdHistogram(longPeriod, shortPeriod, periods);
            _macdIndicators.Add(parameters, indicator);

            return indicator;
        }
#endregion //iMACD

#region iCCI
        private struct CciParameters
        {
            public int Periods;
        }
    
        private Dictionary<CciParameters, CommodityChannelIndex> _cciIndicators = new Dictionary<CciParameters, CommodityChannelIndex>();

        public CommodityChannelIndex CommodityChannelIndex(int periods)
        {
            var cciParameters = new CciParameters { Periods = periods };
            if (_cciIndicators.ContainsKey(cciParameters))
                return _cciIndicators[cciParameters];

            var indicator = _indicatorsAccessor.CommodityChannelIndex(periods);
            _cciIndicators.Add(cciParameters, indicator);

            return indicator;
        }
#endregion //iCCI


#region iStdDev
        private struct stdDevParameters
        {
            public int Periods;
            public DataSeries Source;
            public MovingAverageType MovingAverageType;
        }
    
        private Dictionary<stdDevParameters, StandardDeviation> _stdDevIndicators = new Dictionary<stdDevParameters, StandardDeviation>();

        public StandardDeviation StandardDeviation(DataSeries source, int periods, MovingAverageType movingAverageType)
        {
            var stdDevParameters = new stdDevParameters { Source = source, Periods = periods, MovingAverageType = movingAverageType };
            if (_stdDevIndicators.ContainsKey(stdDevParameters))
                return _stdDevIndicators[stdDevParameters];

            var indicator = _indicatorsAccessor.StandardDeviation(source, periods, 1, movingAverageType);
            _stdDevIndicators.Add(stdDevParameters, indicator);

            return indicator;
        }
#endregion //iStdDev
    }

    static class Mq4LineStyles
    {
      public static LineStyle ToLineStyle(int style)
      {
        switch (style)
        {
          case 1: 
            return LineStyle.Lines;
          case 2: 
            return LineStyle.Dots;
          case 3: 
          case 4: 
            return LineStyle.LinesDots;
          default:
            return LineStyle.Solid;
        }
      }
    }

    static class Mq4Colors
    {
        public static Colors GetColorByInteger(int integer)
        {
            switch (integer)
            {
                case 16777215: return Colors.White; case 16448255: return Colors.Snow; case 16449525: return Colors.MintCream; case 16118015: return Colors.LavenderBlush; case 16775408: return Colors.AliceBlue; case 15794160: return Colors.Honeydew; case 15794175: return Colors.Ivory; case 16119285: return Colors.WhiteSmoke; case 15136253: return Colors.OldLace; case 14804223: return Colors.MistyRose; case 16443110: return Colors.Lavender; case 15134970: return Colors.Linen; case 16777184: return Colors.LightCyan; case 14745599: return Colors.LightYellow; case 14481663: return Colors.Cornsilk; case 14020607: return Colors.PapayaWhip; case 14150650: return Colors.AntiqueWhite; case 14480885: return Colors.Beige; case 13499135: return Colors.LemonChiffon; case 13495295: return Colors.BlanchedAlmond; case 12903679: return Colors.Bisque; case 13353215: return Colors.Pink; case 12180223: return Colors.PeachPuff; case 14474460: return Colors.Gainsboro; case 12695295: return Colors.LightPink; case 11920639: return Colors.Moccasin; case 11394815: return Colors.NavajoWhite; case 11788021: return Colors.Wheat; case 13882323: return Colors.LightGray; case 15658671: return Colors.PaleTurquoise; case 11200750: return Colors.PaleGoldenrod; case 15130800: return Colors.PowderBlue; case 14204888: return Colors.Thistle; case 10025880: return Colors.PaleGreen; case 15128749: return Colors.LightBlue; case 14599344: return Colors.LightSteelBlue; case 16436871: return Colors.LightSkyBlue; case 12632256: return Colors.Silver; case 13959039: return Colors.Aquamarine; case 9498256: return Colors.LightGreen; case 9234160: return Colors.Khaki; case 14524637: return Colors.Plum; case 8036607: return Colors.LightSalmon; case 15453831: return Colors.SkyBlue; case 8421616: return Colors.LightCoral; case 15631086: return Colors.Violet; case 7504122: return Colors.Salmon; case 11823615: return Colors.HotPink; case 8894686: return Colors.BurlyWood; case 8034025: return Colors.DarkSalmon; case 9221330: return Colors.Tan; case 15624315: return Colors.MediumSlateBlue; case 6333684: return Colors.SandyBrown; case 11119017: return Colors.DarkGray; case 15570276: return Colors.CornflowerBlue; case 5275647: return Colors.Coral; case 9662683: return Colors.PaleVioletRed; case 14381203: return Colors.MediumPurple; case 14053594: return Colors.Orchid; case 9408444: return Colors.RosyBrown; case 4678655: return Colors.Tomato; case 9419919: return Colors.DarkSeaGreen; case 11193702: return Colors.MediumAquamarine; case 3145645: return Colors.GreenYellow; case 13850042: return Colors.MediumOrchid; case 6053069: return Colors.IndianRed; case 7059389: return Colors.DarkKhaki; case 13458026: return Colors.SlateBlue; case 14772545: return Colors.RoyalBlue; case 13688896: return Colors.Turquoise; case 16748574: return Colors.DodgerBlue; case 13422920: return Colors.MediumTurquoise; case 9639167: return Colors.DeepPink; case 10061943: return Colors.LightSlateGray; case 14822282: return Colors.BlueViolet; case 4163021: return Colors.Peru; case 9470064: return Colors.SlateGray; case 8421504: return Colors.Gray; case 255: return Colors.Red; case 16711935: return Colors.Magenta; case 16711680: return Colors.Blue; case 16760576: return Colors.DeepSkyBlue; case 16776960: return Colors.Aqua; case 8388352: return Colors.SpringGreen; case 65280: return Colors.Lime; case 65407: return Colors.Chartreuse; case 65535: return Colors.Yellow; case 55295: return Colors.Gold; case 42495: return Colors.Orange; case 36095: return Colors.DarkOrange; case 17919: return Colors.OrangeRed; case 3329330: return Colors.LimeGreen; case 3329434: return Colors.YellowGreen; case 13382297: return Colors.DarkOrchid; case 10526303: return Colors.CadetBlue; case 64636: return Colors.LawnGreen; case 10156544: return Colors.MediumSpringGreen; case 2139610: return Colors.Goldenrod; case 11829830: return Colors.SteelBlue; case 3937500: return Colors.Crimson; case 1993170: return Colors.Chocolate; case 7451452: return Colors.MediumSeaGreen; case 8721863: return Colors.MediumVioletRed; case 13828244: return Colors.DarkViolet; case 11186720: return Colors.LightSeaGreen; case 6908265: return Colors.DimGray; case 13749760: return Colors.DarkTurquoise; case 2763429: return Colors.Brown; case 13434880: return Colors.MediumBlue; case 2970272: return Colors.Sienna; case 9125192: return Colors.DarkSlateBlue; case 755384: return Colors.DarkGoldenrod; case 5737262: return Colors.SeaGreen; case 2330219: return Colors.OliveDrab; case 2263842: return Colors.ForestGreen; case 1262987: return Colors.SaddleBrown; case 3107669: return Colors.DarkOliveGreen; case 9109504: return Colors.DarkBlue; case 7346457: return Colors.MidnightBlue; case 8519755: return Colors.Indigo; case 128: return Colors.Maroon; case 8388736: return Colors.Purple; case 8388608: return Colors.Navy; case 8421376: return Colors.Teal; case 32768: return Colors.Green; case 32896: return Colors.Olive; case 5197615: return Colors.DarkSlateGray; case 25600: return Colors.DarkGreen; case 0:
                default:
                    return Colors.Black;
            }
        }
    }

    class DataSeriesExtremums
    {
        private int? _lastCheckedIndex;
        private readonly DataSeries _dataSeries;
        private double _min = double.MaxValue;
        private double _max = double.MinValue;

        public DataSeriesExtremums(DataSeries dataSeries)
        {
            _dataSeries = dataSeries;
        }

        private void UpdateMinAndMax()
        {
            var indexFrom = _lastCheckedIndex != null ? _lastCheckedIndex.Value + 1 : 0;
            for (var i = indexFrom; i < _dataSeries.Count - 1; i++)
            {
                if (_dataSeries[i] < _min)
                    _min = _dataSeries[i];
                if (_dataSeries[i] > _max)
                    _max = _dataSeries[i];
                _lastCheckedIndex = i;
            }
        }

        public double Min
        {
            get 
            {
                UpdateMinAndMax();
                return _min;
            }
        }

        public double Max
        {
            get 
            {
                UpdateMinAndMax();
                return _max;
            }
        }
    }