	[Conditional("DayOfWeek")]	    
    int DayOfWeek()
    {
      return (int)Server.Time.DayOfWeek;
    }

	[Conditional("DayOfYear")]
    int DayOfYear()
    {
      return Server.Time.DayOfYear;
    }

	[Conditional("Hour")]
    int Hour()
    {
      return Server.Time.Hour;
    }

	[Conditional("Minute")]
    int Minute()
    {
      return Server.Time.Minute;
    }
        
	[Conditional("Month")]
    int Month()
    {
      return Server.Time.Month;
    }
            
	[Conditional("Seconds")]
    int Seconds()
    {
      return Server.Time.Second;
    }

	[Conditional("TimeCurrent", "CurTime")]
    int TimeCurrent()
    {
      return Mq4TimeSeries.ToInteger(Server.Time);
    }

	[Conditional("CurTime")]
    int CurTime()
    {
      return TimeCurrent();
    }

	[Conditional("TimeDay")]
    int TimeDay(int time)
    {
      return Mq4TimeSeries.ToDateTime(time).Day;
    }

	[Conditional("TimeDayOfWeek")]
    int TimeDayOfWeek(int time)
    {
      return (int)Mq4TimeSeries.ToDateTime(time).DayOfWeek;
    }

	[Conditional("TimeDayOfYear")]
    int TimeDayOfYear(int time)
    {
      return Mq4TimeSeries.ToDateTime(time).DayOfYear;
    }

	[Conditional("TimeHour")]
    int TimeHour(int time)
    {
      return Mq4TimeSeries.ToDateTime(time).Hour;
    }

	[Conditional("TimeLocal")]
    int TimeLocal()
    {
      return Mq4TimeSeries.ToInteger(Server.Time);
    }
        
	[Conditional("TimeMinute")]
    int TimeMinute(int time)
    {
      return Mq4TimeSeries.ToDateTime(time).Minute;
    }       

	[Conditional("TimeMonth")]
    int TimeMonth(int time)
    {
      return Mq4TimeSeries.ToDateTime(time).Month;
    }
      
	[Conditional("TimeSeconds")]
    int TimeSeconds(int time)
    {
      return Mq4TimeSeries.ToDateTime(time).Second;
    }

	[Conditional("TimeYear")]
    int TimeYear(int time)
    {
      return Mq4TimeSeries.ToDateTime(time).Year;
    }

	[Conditional("Year")]
    int Year()
    {
      return Server.Time.Year;
    }

	[Conditional("Day")]
    int Day()
    {
      return Server.Time.Day;
    }