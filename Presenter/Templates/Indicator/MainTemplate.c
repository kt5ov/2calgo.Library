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

#Conditional_Part_PLACE_HOLDER#
}