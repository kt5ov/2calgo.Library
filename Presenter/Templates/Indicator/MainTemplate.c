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
using System.Text.RegularExpressions;
using System.Collections;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO;
using System.Reflection;
using System.Threading;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace cAlgo.Indicators
{
    [Indicator(ScalePrecision = 5, AutoRescale = false, IsOverlay = #IsDrawingOnChartWindow_PLACE_HOLDER#)]
	[Levels(#Levels_PLACE_HOLDER#)]
    public class ConvertedIndicator : Indicator
    {
[Conditional("SendMail")]
//{
		[Parameter("Email from", DefaultValue = "your_email@gmail.com")]
        public string EmailAddressFrom { get; set; }

        [Parameter("Email to", DefaultValue = "your_email@gmail.com")]
        public string EmailAddressTo { get; set; }
//}

#Parameters_PLACE_HOLDER#

#Lines_declarations_PLACE_HOLDER#
     
#Mq4Fields_PLACE_HOLDER#

#Mq4Functions_PLACE_HOLDER#

#ColorParameters_PLACE_HOLDER#
        
        protected override void Initialize()
        {
            _closeExtremums = new DataSeriesExtremums(MarketSeries.Close);
#Initialize_buffers_PLACE_HOLDER#

            Open = new Mq4MarketDataSeries(MarketSeries.Open);
            High = new Mq4MarketDataSeries(MarketSeries.High);
            Low = new Mq4MarketDataSeries(MarketSeries.Low);
            Close = new Mq4MarketDataSeries(MarketSeries.Close);
            Volume = new Mq4MarketDataSeries(MarketSeries.TickVolume);
            Median = new Mq4MarketDataSeries(MarketSeries.Median);
            Time = new Mq4TimeSeries(MarketSeries.OpenTime);

            _cashedStandardIndicators = new CashedStandardIndicators(Indicators);
            _mq4ChartObjects = new Mq4ChartObjects(ChartObjects, MarketSeries.OpenTime);
			_mq4ArrayToDataSeriesConverterFactory = new Mq4ArrayToDataSeriesConverterFactory(() => CreateDataSeries());
            Debug.Initialize(m => Print(m)); 
        }

        int _currentIndex;
        CashedStandardIndicators _cashedStandardIndicators;
        Mq4ChartObjects _mq4ChartObjects;
#Inverted_buffers_declarations_PLACE_HOLDER#
        Mq4MarketDataSeries Open;
        Mq4MarketDataSeries High;
        Mq4MarketDataSeries Low;
        Mq4MarketDataSeries Close;
        Mq4MarketDataSeries Median;
        Mq4MarketDataSeries Volume;
        Mq4TimeSeries Time;

        DataSeriesExtremums _closeExtremums;
        readonly List<Mq4OutputDataSeries> _allBuffers = new List<Mq4OutputDataSeries>();
		Mq4ArrayToDataSeriesConverterFactory _mq4ArrayToDataSeriesConverterFactory;

    public override void Calculate(int index)
    {
        _currentIndex = index;
#Buffers_SetCurrentIndex_PLACE_HOLDER#
		Time.SetCurrentIndex(index);

        if (index == 100)
            Mq4Init();
        if (IsRealTime || MarketSeries.OpenTime[index] == LastBarOpenTimeInUtc || IsWeekend && MarketSeries.OpenTime[index] >= LastBarOfWeekOpenTimeInUtc) 
        {       
            Mq4Start();       
            _indicatorCounted = index;
        }
    }

	DateTime LastBarOpenTimeInUtc
	{
		get
		{
			var date = Server.Time;
			if (TimeFrame == TimeFrame.Minute)
				return new DateTime(date.Year, date.Month, date.Day, date.Hour, date.Minute, 0, date.Kind);
			if (TimeFrame == TimeFrame.Minute2)
				return new DateTime(date.Year, date.Month, date.Day, date.Hour, date.Minute - date.Minute % 2, 0, date.Kind);
			if (TimeFrame == TimeFrame.Minute3)
				return new DateTime(date.Year, date.Month, date.Day, date.Hour, date.Minute - date.Minute % 3, 0, date.Kind);
			if (TimeFrame == TimeFrame.Minute4)
				return new DateTime(date.Year, date.Month, date.Day, date.Hour, date.Minute - date.Minute % 4, 0, date.Kind);
			if (TimeFrame == TimeFrame.Minute5)
				return new DateTime(date.Year, date.Month, date.Day, date.Hour, date.Minute - date.Minute % 5, 0, date.Kind);
			if (TimeFrame == TimeFrame.Minute10)
				return new DateTime(date.Year, date.Month, date.Day, date.Hour, date.Minute - date.Minute % 10, 0, date.Kind);
			if (TimeFrame == TimeFrame.Minute15)
				return new DateTime(date.Year, date.Month, date.Day, date.Hour, date.Minute - date.Minute % 15, 0, date.Kind);
			if (TimeFrame == TimeFrame.Minute30)
				return new DateTime(date.Year, date.Month, date.Day, date.Hour, date.Minute - date.Minute % 30, 0, date.Kind);
			if (TimeFrame == TimeFrame.Hour)
				return new DateTime(date.Year, date.Month, date.Day, date.Hour, 0, 0, date.Kind);
			if (TimeFrame == TimeFrame.Hour4)
				return new DateTime(date.Year, date.Month, date.Day, date.Hour - date.Hour % 4, 0, 0, date.Kind);
			if (TimeFrame == TimeFrame.Hour12)
				return new DateTime(date.Year, date.Month, date.Day, date.Hour - date.Hour % 12, 0, 0, date.Kind);

			if (TimeFrame == TimeFrame.Daily)
				return new DateTime(date.Year, date.Month, date.Day, 0, 0, 0, date.Kind);
			if (TimeFrame == TimeFrame.Weekly)
				return date.Date.AddDays((double) System.DayOfWeek.Sunday - (double) date.Date.DayOfWeek);
			
			return new DateTime(date.Year, date.Month, 1, 0, 0, 0, date.Kind);
		}
	}

    DateTime LastBarOfWeekOpenTimeInEE
    {
        get
        {
            var now = ConvertUtcToEEuropeTime(Server.Time);
            var saturday = now.Date;
            while (saturday.DayOfWeek != System.DayOfWeek.Saturday)
                saturday = saturday.AddDays(-1);
			            
            if (TimeFrame <= TimeFrame.Daily)
                return saturday.AddMinutes(-Period());    
            if (TimeFrame == TimeFrame.Weekly)        
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
            var now = ConvertUtcToEEuropeTime(Server.Time);                     
            return now.DayOfWeek == System.DayOfWeek.Sunday || now.DayOfWeek == System.DayOfWeek.Saturday;
        }
    }

	MarketSeries GetSeries(int period)
	{
		var timeFrame = PeriodToTimeFrame(period);
		return MarketData.GetSeries(timeFrame);
	}

	private DataSeries ToMarketSeries(int timeframe, int constant)
    {
		var series = GetSeries(timeframe);
        switch (constant)
        {
            case PRICE_OPEN:
                return series.Open;
            case PRICE_HIGH:
                return series.High;
            case PRICE_LOW:
                return series.Low;
            case PRICE_CLOSE:
                return series.Close;
            case PRICE_MEDIAN:
                return series.Median;       
            case PRICE_TYPICAL:
                return series.Typical;    
            case PRICE_WEIGHTED:
                return series.Weighted;
        }
        throw new NotImplementedException("Converter doesn't support working with this type of AppliedPrice");
    }   

#InnerParts_PLACE_HOLDER#
	}

#OuterParts_PLACE_HOLDER#

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

	static class EventExtensions
	{
		public static void Raise<T1, T2>(this Action<T1, T2> action, T1 arg1, T2 arg2)
		{
			if (action != null)
				action(arg1, arg2);
		}
	}
}