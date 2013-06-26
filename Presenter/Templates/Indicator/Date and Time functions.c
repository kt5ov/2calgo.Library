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
