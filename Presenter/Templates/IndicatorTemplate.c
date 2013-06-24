// ------------------------------------------------------------
// Paste this code into your cAlgo editor
// -----------------------------------------------------------
using System;
using System.Collections.Generic;
using cAlgo.API;
using cAlgo.API.Indicators;
using cAlgo.API.Internals;
using System.Linq;
using System.Text;

namespace cAlgo.Indicators
{
    [Indicator(ScalePrecision = 5, IsOverlay = #IsDrawingOnChartWindow_PLACE_HOLDER#)]
    public class ConvertedIndicator : Indicator
    {
#Parameters_PLACE_HOLDER#

#Lines_declarations_PLACE_HOLDER#

        private int Mq4Start()
        {         
#Start_function_PLACE_HOLDER#
        }    

        private void Mq4Init()
        {         
#Init_function_PLACE_HOLDER#
        }    

#Mq4Fields_PLACE_HOLDER#

#Mq4Functions_PLACE_HOLDER#
        
        protected override void Initialize()
        {
            _closeExtremums = new DataSeriesExtremums(MarketSeries.Close);
#Initialize_buffers_PLACE_HOLDER#

            Open = new InvertedDataSeries(MarketSeries.Open);
            High = new InvertedDataSeries(MarketSeries.High);
            Low = new InvertedDataSeries(MarketSeries.Low);
            Close = new InvertedDataSeries(MarketSeries.Close);
            Volume = new InvertedDataSeries(MarketSeries.TickVolume);
            Median = new InvertedDataSeries(MarketSeries.Median);
            Time = new Mq4TimeSeries(MarketSeries.OpenTime);

            _cashedStandardIndicators = new CashedStandardIndicators(Indicators);
            _mq4ChartObjects = new Mq4ChartObjects(ChartObjects, MarketSeries.OpenTime);
            Debug.Initialize(m => Print(m));
        }

        private int _currentIndex;
        private CashedStandardIndicators _cashedStandardIndicators;
        private Mq4ChartObjects _mq4ChartObjects;
#Inverted_buffers_declarations_PLACE_HOLDER#
        private InvertedDataSeries Open;
        private InvertedDataSeries High;
        private InvertedDataSeries Low;
        private InvertedDataSeries Close;
        private InvertedDataSeries Median;
        private InvertedDataSeries Volume;
        private Mq4TimeSeries Time;

        private DataSeriesExtremums _closeExtremums;
        private readonly List<Mq4DataSeries> _allBuffers = new List<Mq4DataSeries>();

        public override void Calculate(int index)
        {
            _currentIndex = index;
#Buffers_SetCurrentIndex_PLACE_HOLDER#

            Open.SetCurrentIndex(index);
            High.SetCurrentIndex(index);
            Low.SetCurrentIndex(index);
            Close.SetCurrentIndex(index);
            Median.SetCurrentIndex(index);
            Volume.SetCurrentIndex(index);
            Time.SetCurrentIndex(index);

            if (index == 100)
                Mq4Init();
            if (IsRealTime || index > 100 && IsWeekend && MarketSeries.OpenTime[index] >= LastBarOfWeekOpenTimeInUtc) 
            {       
                Mq4Start();       
                _indicatorCounted = index;
            }
       }

    DateTime LastBarOfWeekOpenTimeInEE
    {
        get
        {
            var now = ConvertUtcToEEuropeTime(DateTime.UtcNow);
            var saturday = now.Date;
            while (saturday.DayOfWeek != System.DayOfWeek.Saturday)
                saturday = saturday.AddDays(-1);

            var dailyPeriod = 24 * 60;
            if (Period() <= dailyPeriod)
                return saturday.AddMinutes(-Period());    
            if (Period() == dailyPeriod * 7)        
                return saturday.AddDays(-6);

            return new DateTime(now.Year, now.Month, 1);
        }
    }

    DateTime LastBarOfWeekOpenTimeInUtc
    {
        get
        {
            return ConvertEEuropeTimeToUtc(LastBarOfWeekOpenTimeInEE);
        }
    }

    TimeZoneInfo _easternEuropeTimeZone;
    TimeZoneInfo EasternEuropeTimeZone
    {
        get 
        {
            const string easternZoneId = "E. Europe Standard Time";
            if (_easternEuropeTimeZone == null)
                _easternEuropeTimeZone = TimeZoneInfo.FindSystemTimeZoneById(easternZoneId);
            return _easternEuropeTimeZone;
        }
    }


    DateTime ConvertUtcToEEuropeTime(DateTime dateTime)
    {       
        return TimeZoneInfo.ConvertTimeFromUtc(dateTime, EasternEuropeTimeZone);
    }

    DateTime ConvertEEuropeTimeToUtc(DateTime dateTime)
    {       
        return TimeZoneInfo.ConvertTimeToUtc(dateTime, EasternEuropeTimeZone);
    }

    bool IsWeekend
    {
        get 
        {
            var now = ConvertUtcToEEuropeTime(DateTime.UtcNow);                     
            return now.DayOfWeek == System.DayOfWeek.Sunday || now.DayOfWeek == System.DayOfWeek.Saturday;
        }
    }

    #region InitFunctions
    
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

    #endregion

#region Predefined variables

    private int _indicatorCounted;
        private int IndicatorCounted()
        {
            return _indicatorCounted;
        }

        private int Bars
        {
            get { return MarketSeries.Close.Count; }
        }

    private int Digits
    {
      get
      {
        if (Symbol == null)
          return 0;
        return Symbol.Digits;
      }
    }
      
        Mq4Double Point
        {
          get
          {
        if (Symbol == null)
          return 0.00001;

            return Symbol.PointSize;
        }
        }

        private int? _period;
        private int Period()
        {         
        if (_period == null)
        {
          var counters = new Dictionary<int, int>();
          for (var i = 1; i < MarketSeries.Close.Count - 1; i++)
          {
            var timeSpan = (int)(MarketSeries.OpenTime[i] - MarketSeries.OpenTime[i - 1]).TotalMinutes;
            if (!counters.ContainsKey(timeSpan))
              counters.Add(timeSpan, 0);
            counters[timeSpan]++;
          }
          _period = counters.OrderByDescending(kvp => kvp.Value)
                    .Select(kvp => kvp.Key)
                    .First();
          _period = Math.Min(_period.Value, 43200);
        }
        return _period.Value;
        }
#endregion

    static class Debug
    {
        private static Action<string> _printAction;
        public static void Initialize(Action<string> printAction)
        {
            _printAction = printAction;
        }

        public static void WriteLine(string message)
        {
            _printAction(message);
        }

        public static void WriteLine(string format, params object[] args)
        {
            _printAction(string.Format(format, args));
        }
    }

#region DataSeries

    internal class Mq4TimeSeries
    {
        private readonly TimeSeries _timeSeries;
        private int _currentIndex;
        private static readonly DateTime StartDateTime = new DateTime(1970, 1, 1);

        public Mq4TimeSeries(TimeSeries timeSeries)
        { 
            _timeSeries = timeSeries;
        }

        public void SetCurrentIndex(int index)
        {
            _currentIndex = index;
        }       

        public static int ToInteger(DateTime dateTime)
        {
          return (int)(dateTime - StartDateTime).TotalSeconds;
        }
            
        public static DateTime ToDateTime(int seconds)
        {
          return StartDateTime.AddSeconds(seconds);
        }
                
        public int this[int index]
        {
            get 
            { 
                if (index >= _timeSeries.Count)
                  return 0;
                  
                DateTime dateTime = _timeSeries[_currentIndex - index];

                return ToInteger(dateTime); 
            }
        }
    }

    internal class InvertedDataSeries
    {
        private DataSeries _dataSeries;
        private int _currentIndex;

        public InvertedDataSeries(DataSeries dataSeries)
        {
            _dataSeries = dataSeries;
        }

        public void SetCurrentIndex(int index)
        {
            _currentIndex = index;
        }

        public double this[int index]
        {
            get { return _dataSeries[_currentIndex - index]; }
        }
    }

    class Mq4DataSeries
    {
        public IndicatorDataSeries OutputDataSeries { get; private set; }
        private readonly NormalIndicatorDataSeries _originalValues = new NormalIndicatorDataSeries();
        private int _currentIndex;
        private int _shift;
        private double _emptyValue = double.NaN;
        private readonly DataSeriesExtremums _closeExtremums;
        private readonly ChartObjects _chartObjects;
        private readonly List<int> _overlapLineStartIndexes = new List<int>();

        public Mq4DataSeries(IndicatorDataSeries outputDataSeries, DataSeriesExtremums closeExtremums, ChartObjects chartObjects)
        {
            OutputDataSeries = outputDataSeries;
            _closeExtremums = closeExtremums;
            _chartObjects = chartObjects;
        }

        public int Count
        {
            get 
            {
                return OutputDataSeries.Count;
            }
        }

        public void SetCurrentIndex(int index)
        {
            _currentIndex = index;
        }        
        
        public void SetShift(int shift)
        {
          _shift = shift;
        }
        
        public void SetEmptyValue(double emptyValue)
        {
          _emptyValue = emptyValue;
        }

        public double this[int index]
        {
            get 
            { 
                if (index >= _originalValues.Count)
                  return 0;
                        
                return _originalValues[_currentIndex - index + _shift];
            }
            set 
            { 
                var indexToSet = _currentIndex - index + _shift;                
                _originalValues[indexToSet] = value;

                var valueToSet = value;
                if (valueToSet == _emptyValue)
                  valueToSet = double.NaN;

                if (indexToSet < 0)
                  return;

                if (#IsDrawingOnChartWindow_PLACE_HOLDER#)
                {
                    var validRange = _closeExtremums.Max - _closeExtremums.Min;                
                    if (value > _closeExtremums.Max + validRange || value < _closeExtremums.Min - validRange)
                        return;
                }

                if (!double.IsNaN(valueToSet) && double.IsNaN(OutputDataSeries[indexToSet - 1]))
                {
                    int startIndex;
                    for (startIndex = indexToSet - 1; startIndex >= 0; startIndex--)
                    {
                        if (!double.IsNaN(OutputDataSeries[startIndex]))
                            break;
                    }
                    if (startIndex > 0)
                    {
                        RemoveOverlapLinesSinceIndex(startIndex);

                        _chartObjects.DrawLine(GetOverlapLineName(startIndex), startIndex, OutputDataSeries[startIndex], indexToSet, valueToSet, Colors.Black, 3);
                        _overlapLineStartIndexes.Add(startIndex);
                    }                    
                }

                OutputDataSeries[indexToSet] = valueToSet; 
            }
        }

        private void RemoveOverlapLinesSinceIndex(int index)
        {
            foreach (var startIndex in _overlapLineStartIndexes.ToArray())                    
            {
                if (startIndex >= index)
                {
                    _overlapLineStartIndexes.Remove(startIndex);
                    _chartObjects.RemoveObject(GetOverlapLineName(startIndex));
                }
            }
        }

        private string GetOverlapLineName(int startIndex)
        {
            return string.Format("Overlapline {0} {1}", GetHashCode(), startIndex);
        }
    }

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

#endregion //DataSeries

#region MQ4 Constants
        const bool True = true;
        const bool False = false;
        const bool TRUE = true;
        const bool FALSE = false;                
        const string NULL = null;
        const int EMPTY = -1;
        const double EMPTY_VALUE = double.NaN;
        const int WHOLE_ARRAY = 0;

        const int MODE_SMA = 0; //Simple moving average
        const int MODE_EMA = 1; //Exponential moving average,
        const int MODE_SMMA = 2; //Smoothed moving average,
        const int MODE_LWMA = 3; //Linear weighted moving average. 
    
        const int PRICE_CLOSE = 0; //Close price. 
        const int PRICE_OPEN = 1; //Open price. 
        const int PRICE_HIGH = 2; //High price. 
        const int PRICE_LOW = 3; //Low price. 
        const int PRICE_MEDIAN = 4; //Median price, (high+low)/2. 
        const int PRICE_TYPICAL = 5; //Typical price, (high+low+close)/3. 
        const int PRICE_WEIGHTED = 6; //Weighted close price, (high+low+close+close)/4. 

    const int DRAW_LINE = 0;
    const int DRAW_SECTION = 1;
    const int DRAW_HISTOGRAM = 2;
    const int DRAW_ARROW = 3;
    const int DRAW_ZIGZAG = 4;
    const int DRAW_NONE = 12;

    const int STYLE_SOLID = 0;
    const int STYLE_DASH = 1;
    const int STYLE_DOT = 2;
    const int STYLE_DASHDOT = 3;
    const int STYLE_DASHDOTDOT = 4;

    const int MODE_OPEN = 0;
    const int MODE_LOW = 1;
    const int MODE_HIGH = 2;
    const int MODE_CLOSE = 3;
    const int MODE_VOLUME = 4;
    const int MODE_TIME = 5;
    const int MODE_BID = 9;
    const int MODE_ASK = 10;
    //const int MODE_POINT = 11;
    const int MODE_DIGITS = 12;
    /*const int MODE_SPREAD = 13;
    const int MODE_STOPLEVEL = 14;
    const int MODE_LOTSIZE = 15;
    const int MODE_TICKVALUE = 16;
    const int MODE_TICKSIZE = 17;
    const int MODE_SWAPLONG = 18;
    const int MODE_SWAPSHORT = 19;
    const int MODE_STARTING = 20;
    const int MODE_EXPIRATION = 21;
    const int MODE_TRADEALLOWED = 22;
    const int MODE_MINLOT = 23;
    const int MODE_LOTSTEP = 24;
    const int MODE_MAXLOT = 25;
    const int MODE_SWAPTYPE = 26;
    const int MODE_PROFITCALCMODE = 27;
    const int MODE_MARGINCALCMODE = 28;
    const int MODE_MARGININIT = 29;
    const int MODE_MARGINMAINTENANCE = 30;
    const int MODE_MARGINHEDGED = 31;
    const int MODE_MARGINREQUIRED = 32;
    const int MODE_FREEZELEVEL = 33;*/

    const int OBJ_VLINE = 0;
        const int OBJ_HLINE = 1;
        const int OBJ_TREND = 2;
        /*const int OBJ_TRENDBYANGLE = 3;
        const int OBJ_REGRESSION = 4;
        const int OBJ_CHANNEL = 5;
        const int OBJ_STDDEVCHANNEL = 6;
        const int OBJ_GANNLINE = 7;
        const int OBJ_GANNFAN = 8;
        const int OBJ_GANNGRID = 9;
        const int OBJ_FIBO = 10;
        const int OBJ_FIBOTIMES = 11;
        const int OBJ_FIBOFAN = 12;
        const int OBJ_FIBOARC = 13;
        const int OBJ_EXPANSION = 14;
        const int OBJ_FIBOCHANNEL = 15;*/
        const int OBJ_RECTANGLE = 16;
        /*const int OBJ_TRIANGLE = 17;
        const int OBJ_ELLIPSE = 18;
        const int OBJ_PITCHFORK = 19;
        const int OBJ_CYCLES = 20;*/
        const int OBJ_TEXT = 21;
        //const int OBJ_ARROW = 22;
        const int OBJ_LABEL = 23;
        
        const int OBJPROP_TIME1 = 0;
        const int OBJPROP_PRICE1 = 1;
        const int OBJPROP_TIME2 = 2;
        const int OBJPROP_PRICE2 = 3;
        const int OBJPROP_TIME3 = 4;
        const int OBJPROP_PRICE3 = 5;
        const int OBJPROP_COLOR = 6;
        const int OBJPROP_STYLE = 7;
        const int OBJPROP_WIDTH = 8;
        const int OBJPROP_BACK = 9;
        const int OBJPROP_RAY = 10;
        const int OBJPROP_ELLIPSE = 11;
        const int OBJPROP_SCALE = 12;
        const int OBJPROP_ANGLE = 13;
        const int OBJPROP_ARROWCODE = 14;
        const int OBJPROP_TIMEFRAMES = 15;
        const int OBJPROP_DEVIATION = 16;
        const int OBJPROP_FONTSIZE = 100;
        const int OBJPROP_CORNER = 101;
        const int OBJPROP_XDISTANCE = 102;
        const int OBJPROP_YDISTANCE = 103;
        const int OBJPROP_FIBOLEVELS = 200;
        const int OBJPROP_LEVELCOLOR = 201;
        const int OBJPROP_LEVELSTYLE = 202;
        const int OBJPROP_LEVELWIDTH = 203;

        const int PERIOD_M1 = 1;
    const int PERIOD_M5 = 5;
    const int PERIOD_M15 = 15;
    const int PERIOD_M30 = 30;
    const int PERIOD_H1 = 60;
    const int PERIOD_H4 = 240;
    const int PERIOD_D1 = 1440;
    const int PERIOD_W1 = 10080;
    const int PERIOD_MN1 = 43200;

    const int TIME_DATE = 1;
    const int TIME_MINUTES = 2;
    const int TIME_SECONDS = 4;

#region Indicator lines

#endregion //Indicator lines
    const int MODE_MAIN = 0;
    const int MODE_PLUSDI = 1;
    const int MODE_MINUSDI = 2;
    const int MODE_SIGNAL = 1;

    const int MODE_UPPER = 1;
    const int MODE_LOWER = 2;
#endregion //MQ4 Constants

#region Colors
        const int CLR_NONE = 32768; 

        const int White = 16777215;
    const int Snow = 16448255;
    const int MintCream = 16449525;
    const int LavenderBlush = 16118015;
    const int AliceBlue = 16775408;
    const int Honeydew = 15794160;
    const int Ivory = 15794175;
    const int Seashell = 15660543;
    const int WhiteSmoke = 16119285;
    const int OldLace = 15136253;
    const int MistyRose = 14804223;
    const int Lavender = 16443110;
    const int Linen = 15134970;
    const int LightCyan = 16777184;
    const int LightYellow = 14745599;
    const int Cornsilk = 14481663;
    const int PapayaWhip = 14020607;
    const int AntiqueWhite = 14150650;
    const int Beige = 14480885;
    const int LemonChiffon = 13499135;
    const int BlanchedAlmond = 13495295;
    const int LightGoldenrod = 13826810;
    const int Bisque = 12903679;
    const int Pink = 13353215;
    const int PeachPuff = 12180223;
    const int Gainsboro = 14474460;
    const int LightPink = 12695295;
    const int Moccasin = 11920639;
    const int NavajoWhite = 11394815;
    const int Wheat = 11788021;
    const int LightGray = 13882323;
    const int PaleTurquoise = 15658671;
    const int PaleGoldenrod = 11200750;
    const int PowderBlue = 15130800;
    const int Thistle = 14204888;
    const int PaleGreen = 10025880;
    const int LightBlue = 15128749;
    const int LightSteelBlue = 14599344;
    const int LightSkyBlue = 16436871;
    const int Silver = 12632256;
    const int Aquamarine = 13959039;
    const int LightGreen = 9498256;
    const int Khaki = 9234160;
    const int Plum = 14524637;
    const int LightSalmon = 8036607;
    const int SkyBlue = 15453831;
    const int LightCoral = 8421616;
    const int Violet = 15631086;
    const int Salmon = 7504122;
    const int HotPink = 11823615;
    const int BurlyWood = 8894686;
    const int DarkSalmon = 8034025;
    const int Tan = 9221330;
    const int MediumSlateBlue = 15624315;
    const int SandyBrown = 6333684;
    const int DarkGray = 11119017;
    const int CornflowerBlue = 15570276;
    const int Coral = 5275647;
    const int PaleVioletRed = 9662683;
    const int MediumPurple = 14381203;
    const int Orchid = 14053594;
    const int RosyBrown = 9408444;
    const int Tomato = 4678655;
    const int DarkSeaGreen = 9419919;
    const int MediumAquamarine = 11193702;
    const int GreenYellow = 3145645;
    const int MediumOrchid = 13850042;
    const int IndianRed = 6053069;
    const int DarkKhaki = 7059389;
    const int SlateBlue = 13458026;
    const int RoyalBlue = 14772545;
    const int Turquoise = 13688896;
    const int DodgerBlue = 16748574;
    const int MediumTurquoise = 13422920;
    const int DeepPink = 9639167;
    const int LightSlateGray = 10061943;
    const int BlueViolet = 14822282;
    const int Peru = 4163021;
    const int SlateGray = 9470064;
    const int Gray = 8421504;
    const int Red = 255;
    const int Magenta = 16711935;
    const int Blue = 16711680;
    const int DeepSkyBlue = 16760576;
    const int Aqua = 16776960;
    const int SpringGreen = 8388352;
    const int Lime = 65280;
    const int Chartreuse = 65407;
    const int Yellow = 65535;
    const int Gold = 55295;
    const int Orange = 42495;
    const int DarkOrange = 36095;
    const int OrangeRed = 17919;
    const int LimeGreen = 3329330;
    const int YellowGreen = 3329434;
    const int DarkOrchid = 13382297;
    const int CadetBlue = 10526303;
    const int LawnGreen = 64636;
    const int MediumSpringGreen = 10156544;
    const int Goldenrod = 2139610;
    const int SteelBlue = 11829830;
    const int Crimson = 3937500;
    const int Chocolate = 1993170;
    const int MediumSeaGreen = 7451452;
    const int MediumVioletRed = 8721863;
    const int FireBrick = 2237106;
    const int DarkViolet = 13828244;
    const int LightSeaGreen = 11186720;
    const int DimGray = 6908265;
    const int DarkTurquoise = 13749760;
    const int Brown = 2763429;
    const int MediumBlue = 13434880;
    const int Sienna = 2970272;
    const int DarkSlateBlue = 9125192;
    const int DarkGoldenrod = 755384;
    const int SeaGreen = 5737262;
    const int OliveDrab = 2330219;
    const int ForestGreen = 2263842;
    const int SaddleBrown = 1262987;
    const int DarkOliveGreen = 3107669;
    const int DarkBlue = 9109504;
    const int MidnightBlue = 7346457;
    const int Indigo = 8519755;
    const int Maroon = 128;
    const int Purple = 8388736;
    const int Navy = 8388608;
    const int Teal = 8421376;
    const int Green = 32768;
    const int Olive = 32896;
    const int DarkSlateGray = 5197615;
    const int DarkGreen = 25600;
    const int Black = 0;  
#endregion   

#region Math & Trig 
    Mq4Double MathArccos(double x)
    {
      return Math.Acos(x);
    }

    Mq4Double MathArcsin(double x)
    {
      return Math.Asin(x);
    }
    
    Mq4Double MathMax(double value1, double value2) 
    {
      return Math.Max(value1, value2);
    }
        
        Mq4Double MathMin(double value1, double value2) 
    { 
      return Math.Min(value1, value2);
    }

    Mq4Double MathSqrt(double x) 
    {
      return Math.Sqrt(x);
    }

    Mq4Double MathAbs(double x) 
    {
      return Math.Abs(x);
    }

    Mq4Double MathRound(double x) 
    {
      return Math.Round(x);
    }
        
    Mq4Double MathPow(double @base, double exponent)
    {
      return Math.Pow(@base, exponent);
    }

    Mq4Double MathCeil(double x)
    {
      return Math.Ceiling(x);
    }

    Mq4Double MathFloor(double x)
    {
      return Math.Floor(x);
    }
    
    Mq4Double MathLog(double x)
    {
      return Math.Log(x);
    }
    #endregion

#region Conversion functions
        
    string DoubleToStr(double value, int digits)
    {
      return value.ToString("F" + digits);
    }

    Mq4Double NormalizeDouble(double value, int digits)
    {
      return Math.Round(value, digits);
    }

    Mq4Double StrToDouble(string value)
    {
      return double.Parse(value);
    }

    Mq4Double StrToInteger(string value)
    {
      return int.Parse(value);
    }

    string TimeToStr(int value, int mode = TIME_DATE|TIME_MINUTES)
    {
      var formatString = "";
      if ((mode & TIME_DATE) != 0)
        formatString += "yyyy.MM.dd ";
      if ((mode & TIME_SECONDS) != 0)
        formatString += "HH:mm:ss";             
      else if ((mode & TIME_MINUTES) != 0)
        formatString += "HH:mm";      
      formatString = formatString.Trim();

      return Mq4TimeSeries.ToDateTime(value).ToString(formatString);
    }

#endregion

#region Window functions 

    int WindowFind(string name)
    {
        return 0;
    }

#endregion //Window functions

#region String functions
    string StringConcatenate(params object[] parameters)
    {
      var stringBuilder = new StringBuilder();
      foreach (var parameter in parameters)
        stringBuilder.Append(parameter.ToString());       
      return stringBuilder.ToString();
    }

    int StringFind(string text, string matched_text, int start = 0)
    {
      return text.IndexOf(matched_text, start);
    }

    int StringLen(string text)
    {
      return text.Length;
    }

    string StringSubstr(string text, int start, int length = 0)
    {
      return text.Substring(start, length);
    }

    string StringTrimLeft(string text)
    {
      return text.TrimStart();
    }

    string StringTrimRight(string text)
    {
      return text.TrimEnd();
    }


#endregion //String functions

#region Date & Time functions
    
    int DayOfWeek()
    {
      return (int)DateTime.Now.DayOfWeek;
    }

    int DayOfYear()
    {
      return DateTime.Now.DayOfYear;
    }

    int Hour()
    {
      return DateTime.Now.Hour;
    }

    int Minute()
    {
      return DateTime.Now.Minute;
    }
        
    int Month()
    {
      return DateTime.Now.Month;
    }
            
    int Seconds()
    {
      return DateTime.Now.Second;
    }

    int TimeCurrent()
    {
      return Mq4TimeSeries.ToInteger(DateTime.Now);
    }

    int CurTime()
    {
      return TimeCurrent();
    }

    int TimeDay(int time)
    {
      return Mq4TimeSeries.ToDateTime(time).Day;
    }

    int TimeDayOfWeek(int time)
    {
      return (int)Mq4TimeSeries.ToDateTime(time).DayOfWeek;
    }

    int TimeDayOfYear(int time)
    {
      return Mq4TimeSeries.ToDateTime(time).DayOfYear;
    }

    int TimeHour(int time)
    {
      return Mq4TimeSeries.ToDateTime(time).Hour;
    }

    int TimeLocal()
    {
      return Mq4TimeSeries.ToInteger(DateTime.Now);
    }
        
    int TimeMinute(int time)
    {
      return Mq4TimeSeries.ToDateTime(time).Minute;
    }       

    int TimeMonth(int time)
    {
      return Mq4TimeSeries.ToDateTime(time).Month;
    }
      
    int TimeSeconds(int time)
    {
      return Mq4TimeSeries.ToDateTime(time).Second;
    }

    int TimeYear(int time)
    {
      return Mq4TimeSeries.ToDateTime(time).Year;
    }

    int Year()
    {
      return DateTime.Now.Year;
    }
#endregion

#region Messages
    const string NotSupportedDifferentTimeFrame = "Converter doesn't support working with different timeframe";
    const string NotSupportedMaShift = "Converter supports only ma_shift = 0";
    const string NotSupportedBandsShift = "Converter supports only bands_shift = 0";
    const string AdxSupportsOnlyClosePrice = "Adx can be applied only to Close price";
#endregion

#region Validation
    void ValidateSymbolAndTimeFrame(string symbol, int timeframe)
    {
      if (timeframe != 0 && timeframe != Period())
                throw new NotImplementedException(NotSupportedDifferentTimeFrame);
    }
#endregion

#region Timeseries access

    int GetHighestIndex(InvertedDataSeries invertedDataSeries, int count, int start)
    {
      var maxIndex = start;
      var endIndex = count == WHOLE_ARRAY ? Bars - 1 : count + start - 1;
      for (var i = start; i <= endIndex; i++)
      {
        if (invertedDataSeries[i] > invertedDataSeries[maxIndex])
          maxIndex = i;
      }
      return maxIndex;
    }

    int GetLowestIndex(InvertedDataSeries invertedDataSeries, int count, int start)
    {
      var minIndex = start;
      var endIndex = count == WHOLE_ARRAY ? Bars - 1 : count + start - 1;
      for (var i = start; i <= endIndex; i++)
      {
        if (invertedDataSeries[i] < invertedDataSeries[minIndex])
          minIndex = i;
      }
      return minIndex;
    }

    int GetExtremeIndex(Func<InvertedDataSeries, int, int, int> extremeFunc, int type, int count, int start)
    {
      switch (type)
      {
        case MODE_OPEN:
          return extremeFunc(Open, count, start);         
        case MODE_HIGH:
          return extremeFunc(High, count, start);
        case MODE_LOW:
          return extremeFunc(Low, count, start);
        case MODE_CLOSE:
          return extremeFunc(Close, count, start);
        case MODE_VOLUME:
          return extremeFunc(Volume, count, start);
        case MODE_TIME:
          return start;
        default:
          throw new ArgumentOutOfRangeException("wrong type for GetExtremeIndex");
      }
    }

    int iHighest(string symbol, int timeframe, int type, int count = WHOLE_ARRAY, int start = 0)
    {
      ValidateSymbolAndTimeFrame(symbol, timeframe);
      return GetExtremeIndex(GetHighestIndex, type, count, start);
    }

    int iLowest(string symbol, int timeframe, int type, int count = WHOLE_ARRAY, int start = 0)
    {
      ValidateSymbolAndTimeFrame(symbol, timeframe);
      return GetExtremeIndex(GetLowestIndex, type, count, start);
    }

    int Highest(string symbol, int timeframe, int type, int count = WHOLE_ARRAY, int start = 0)
    {
      return iHighest(symbol, timeframe, type, count, start);
    }

    int Lowest(string symbol, int timeframe, int type, int count = WHOLE_ARRAY, int start = 0)
    {
      return iLowest(symbol, timeframe, type, count, start);
    }

    double iClose(string symbol, int timeframe, int shift)
    {
      ValidateSymbolAndTimeFrame(symbol, timeframe);
      return Close[shift];
    }

    double iHigh(string symbol, int timeframe, int shift)
    {
      ValidateSymbolAndTimeFrame(symbol, timeframe);
      return High[shift];
    }

    double iLow(string symbol, int timeframe, int shift)
    {
      ValidateSymbolAndTimeFrame(symbol, timeframe);
      return Low[shift];
    }

    double iOpen(string symbol, int timeframe, int shift)
    {
      ValidateSymbolAndTimeFrame(symbol, timeframe);
      return Open[shift];
    }

    double iVolume(string symbol, int timeframe, int shift)
    {
      ValidateSymbolAndTimeFrame(symbol, timeframe);
      return Volume[shift];
    }

    int iTime(string symbol, int timeframe, int shift)
    {
      ValidateSymbolAndTimeFrame(symbol, timeframe);
      return Time[shift];
    }


    int iBarShift(string symbol, int timeframe, int time, bool exact = false)
    {
      ValidateSymbolAndTimeFrame(symbol, timeframe);
      for (var i = 0; i < MarketSeries.Close.Count; i++)
      {
        if (Time[i] == time)
          return i;
        if (Time[i] < time && !exact)
          return i;
      }
      return -1;
    }


#endregion

#region MQ4 Indicators

        private DataSeries ToMarketSeries(int constant)
        {
            switch (constant)
            {
                case PRICE_OPEN:
                    return MarketSeries.Open;
                case PRICE_HIGH:
                    return MarketSeries.High;
                case PRICE_LOW:
                    return MarketSeries.Low;
                case PRICE_CLOSE:
                    return MarketSeries.Close;
                case PRICE_MEDIAN:
                    return MarketSeries.Median;       
                case PRICE_TYPICAL:
                    return MarketSeries.Typical;    
                case PRICE_WEIGHTED:
                    return MarketSeries.Weighted;
            }
            throw new NotImplementedException("Sorry, the Converter doesn't support working with this type of AppliedPrice");
        }   

        private static MovingAverageType ToMaType(int constant)
        {
            switch (constant)
            {
                case MODE_SMA:
                    return MovingAverageType.Simple;
                case MODE_EMA:
                    return MovingAverageType.Exponential;
                case MODE_LWMA:
                    return MovingAverageType.Weighted;
                default:
                  throw new ArgumentOutOfRangeException("Not supported moving average type");
            }
        }

#region iMA
        private double iMA(string symbol, int timeframe, int period, int ma_shift, int ma_method, int applied_price, int shift)
        {
            ValidateSymbolAndTimeFrame(symbol, timeframe);            
            if (ma_shift != 0)
                throw new NotImplementedException(NotSupportedMaShift);

            var series = ToMarketSeries(applied_price);
      
            return CalculateiMA(series, period, ma_method, shift);
        }       
                
        private double iMAOnArray(Mq4DataSeries invertedDataSeries, int total, int period, int ma_shift, int ma_method, int shift) 
        {
          return CalculateiMA(invertedDataSeries.OutputDataSeries, period, ma_method, shift);
        }
        
        private double CalculateiMA(DataSeries dataSeries, int period, int ma_method, int shift)
        {     
            if (ma_method == MODE_SMMA)
            {
                return CalculateWellesWilderSmoothing(dataSeries, period, shift);
            }

            var maType = ToMaType(ma_method);            
            var indicator = _cashedStandardIndicators.MovingAverage(dataSeries, period, maType);

            return indicator.Result[_currentIndex - shift];
        }        
        
        private double CalculateWellesWilderSmoothing(DataSeries dataSeries, int period, int shift)
        {
            var indicator = _cashedStandardIndicators.WellesWilderSmoothing(dataSeries, period);
            
            return indicator.Result[_currentIndex - shift];
        }
#endregion //iMA

#region iRSI
        private double iRSI(string symbol, int timeframe, int period, int applied_price, int shift)
        {
            ValidateSymbolAndTimeFrame(symbol, timeframe);            
            
            var series = ToMarketSeries(applied_price);
      
            return CalculateRsi(series, period, shift);
        }       
                
        private double iRSIOnArray(Mq4DataSeries invertedDataSeries, int total, int period, int shift) 
        {
          return CalculateRsi(invertedDataSeries.OutputDataSeries, period, shift);
        }
        
        private double CalculateRsi(DataSeries dataSeries, int period, int shift)
        {     
            var indicator = _cashedStandardIndicators.RelativeStrengthIndex(dataSeries, period);
            return indicator.Result[_currentIndex - shift];
        }

#endregion //iRSI     

#region iBands
        private double iBands(string symbol, int timeframe, int period, int deviation, int bands_shift, int applied_price, int mode, int shift)
        {
            ValidateSymbolAndTimeFrame(symbol, timeframe);                        
            if (bands_shift != 0)
                throw new NotImplementedException(NotSupportedBandsShift);
            
            var series = ToMarketSeries(applied_price);
      
            return CalculateBands(series, period, deviation, mode, shift);
        }       
                
        private double iBandsOnArray(Mq4DataSeries invertedDataSeries, int total, int period, int deviation, int bands_shift, int mode, int shift) 
        {
            if (bands_shift != 0)
                throw new NotImplementedException(NotSupportedBandsShift);
            
            return CalculateBands(invertedDataSeries.OutputDataSeries, period, deviation, mode, shift);
        }
        
        private double CalculateBands(DataSeries dataSeries, int period, int deviation, int mode, int shift)
        {     
            var indicator = _cashedStandardIndicators.BollingerBands(dataSeries, period, deviation, MovingAverageType.Simple);

            switch (mode)
            {
              case MODE_MAIN:
                return indicator.Main[_currentIndex - shift];
              case MODE_UPPER:
                return indicator.Top[_currentIndex - shift];
              case MODE_LOWER:
                return indicator.Bottom[_currentIndex - shift];
            }

            return 0;
        }

#endregion //iBands   

#region iADX
        private Mq4Double iADX(string symbol, int timeframe, int period, int applied_price, int mode, int shift)
        {
            ValidateSymbolAndTimeFrame(symbol, timeframe);            

            if (applied_price != PRICE_CLOSE)            
              throw new NotImplementedException(AdxSupportsOnlyClosePrice);            
      
            return CalculateAdx(period, mode, shift);
        }      
        
        private Mq4Double CalculateAdx(int period, int mode, int shift)
        {     
            var indicator = _cashedStandardIndicators.DirectionalMovementSystem(period);            
            switch (mode)
            {
              case MODE_MAIN:
                return indicator.ADX[_currentIndex - shift];
              case MODE_PLUSDI:
                return indicator.DIPlus[_currentIndex - shift];
              case MODE_MINUSDI:
                return indicator.DIMinus[_currentIndex - shift];
            }
            return 0;
        }

#endregion //iADX    

#region iATR
        private double iATR(string symbol, int timeframe, int period, int shift)
        {
            ValidateSymbolAndTimeFrame(symbol, timeframe);                        
                  
            return CalculateATR(period, shift);
        }       
                
        private double CalculateATR(int period, int shift)
        {     
            var indicator = _cashedStandardIndicators.ATR(period);

            return indicator.Result[_currentIndex - shift];            
        }

#endregion //iATR   

#region iMACD
        private double iMACD(string symbol, int timeframe, int fast_ema_period, int slow_ema_period, int signal_period, int applied_price, int mode, int shift)
        {
            ValidateSymbolAndTimeFrame(symbol, timeframe);                                    
            if (applied_price != PRICE_CLOSE)
            {
                throw new Exception("cTrader doesn't support source parameter for MACD");
            }
      
            return CalculateMACD(fast_ema_period, slow_ema_period, signal_period, mode, shift);
        }       
        
        private double CalculateMACD(int fast_ema_period, int slow_ema_period, int signal_period, int mode, int shift)
        {     
            var indicator = _cashedStandardIndicators.MACD(fast_ema_period, slow_ema_period, signal_period);

            switch (mode)
            {
              case MODE_MAIN:
                return indicator.Histogram[_currentIndex - shift];
              default:
                return indicator.Signal[_currentIndex - shift];
            }
        }

#endregion //iMACD 

#region iCCI
        private double iCCI(string symbol, int timeframe, int period, int applied_price, int shift)
        {
            ValidateSymbolAndTimeFrame(symbol, timeframe);                        
                        
            var series = ToMarketSeries(applied_price);
      
            return CalculateCCI(series, period, shift);
        }       
                
        private double iCCIOnArray(Mq4DataSeries invertedDataSeries, int period, int shift) 
        {            
            return CalculateCCI(invertedDataSeries.OutputDataSeries, period, shift);
        }
        
        private double CalculateCCI(DataSeries dataSeries, int period, int shift)
        {     
            var indicator = _cashedStandardIndicators.CommodityChannelIndex(period);

            return indicator.Result[_currentIndex - shift];
        }

#endregion //iCCI

#region iStdDev
        private double iStdDev( string symbol, int timeframe, int ma_period, int ma_shift, int ma_method, int applied_price, int shift)
        {
            ValidateSymbolAndTimeFrame(symbol, timeframe);            
            if (ma_shift != 0)
                throw new NotImplementedException(NotSupportedMaShift);

            var series = ToMarketSeries(applied_price);
      
            return CalculateiStdDev(series, ma_period, ma_shift, ma_method, shift);
        }       
                
        private double iStdDevOnArray(Mq4DataSeries invertedDataSeries, int total, int ma_period, int ma_shift, int ma_method, int shift) 
        {
          return CalculateiStdDev(invertedDataSeries.OutputDataSeries, ma_period, ma_shift, ma_method, shift);
        }
        
        private double CalculateiStdDev(DataSeries dataSeries, int ma_period, int ma_shift, int ma_method, int shift)
        {     
            if (ma_shift != 0)
                throw new Exception(NotSupportedMaShift);

            var maType = ToMaType(ma_method);            
            var indicator = _cashedStandardIndicators.StandardDeviation(dataSeries, ma_period, maType);

            return indicator.Result[_currentIndex - shift];
        }        
#endregion //iStdDev

#endregion //MQ4 Indicators
    
#region Common functions
    Mq4Double MarketInfo(string symbol, int type)
    {
      switch (type)
      {
        case MODE_DIGITS:
          return Digits;
        case MODE_ASK:
          return Ask;
        case MODE_BID:
          return Bid;
      }
      return 0;
    }

    Mq4Double Bid
    {
      get 
      {
        if (Symbol == null || double.IsNaN(Symbol.Bid))
          return 0;
        return Symbol.Bid;
      }
    }

    Mq4Double Ask
    {
      get 
      {
        if (Symbol == null || double.IsNaN(Symbol.Ask))
          return 0;
        return Symbol.Ask;
      }
    }

    void Comment(params object[] objects)
        {
          var text = string.Join("", objects.Select(o => o.ToString()));
          ChartObjects.DrawText("top left comment", text, StaticPosition.TopLeft);
        }

    void Alert(params object[] objects)
    {
        var text = string.Join("", objects.Select(o => o.ToString()));      
        Print("Alert: " + text);
    }

    void Sleep(int milliseconds)
    {        
    }

    void PlaySound(string fileName)
    {
        Notifications.PlaySound(fileName);
    }
#endregion //Common functions

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
}