#region Date & Time functions
    
    int DayOfWeek()
    {
      return (int)Server.Time.DayOfWeek;
    }

    int DayOfYear()
    {
      return Server.Time.DayOfYear;
    }

    int Hour()
    {
      return Server.Time.Hour;
    }

    int Minute()
    {
      return Server.Time.Minute;
    }
        
    int Month()
    {
      return Server.Time.Month;
    }
            
    int Seconds()
    {
      return Server.Time.Second;
    }

    int TimeCurrent()
    {
      return Mq4TimeSeries.ToInteger(Server.Time);
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
      return Mq4TimeSeries.ToInteger(Server.Time);
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
      return Server.Time.Year;
    }

    int Day()
    {
      return Server.Time.Day;
    }
#endregion
