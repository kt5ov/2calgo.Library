	[Conditional("CharToStr")]        
	Mq4String CharToStr(int code)
	{
		return ((char)code).ToString();
	}

	[Conditional("DoubleToStr")]        
    Mq4String DoubleToStr(double value, int digits)
    {
      return value.ToString("F" + digits);
    }

	[Conditional("NormalizeDouble")]
    Mq4Double NormalizeDouble(double value, int digits)
    {
      return Math.Round(value, digits);
    }

	[Conditional("StrToDouble")]
    Mq4Double StrToDouble(Mq4String value)
    {
      return double.Parse(value);
    }

	[Conditional("StrToInteger")]
    Mq4Double StrToInteger(Mq4String value)
    {
      return int.Parse(value);
    }

	[Conditional("TimeToStr")]
    Mq4String TimeToStr(int value, int mode = TIME_DATE|TIME_MINUTES)
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
		
	[Conditional("StrToTime")]
	int StrToTime(Mq4String value)
	{
		var dateTime = StrToDateTime(value);
		return Mq4TimeSeries.ToInteger(dateTime);
	}

	[Conditional("StrToDateTime", "StrToTime")]
	//{
	private static readonly Regex TimeRegex = new Regex(@"((?<year>\d+)\.(?<month>\d+)\.(?<day>\d+)){0,1}\s*((?<hour>\d+)\:(?<minute>\d+)){0,1}", RegexOptions.Compiled);
	DateTime StrToDateTime(Mq4String value)
	{
		var dateTime = Server.Time.Date;

		var match = TimeRegex.Match(value);
		if (!match.Success)
			return dateTime;		
				
		if (match.Groups["year"].Value != string.Empty)
		{
			dateTime = new DateTime(int.Parse(match.Groups["year"].Value), int.Parse(match.Groups["month"].Value), int.Parse(match.Groups["day"].Value));
		}		
		if (match.Groups["hour"].Value != string.Empty)
			dateTime = dateTime.AddHours(int.Parse(match.Groups["hour"].Value));
		if (match.Groups["minute"].Value != string.Empty)
			dateTime = dateTime.AddMinutes(int.Parse(match.Groups["minute"].Value));

		return dateTime;
	}
	//}
	
	int ToMq4ErrorCode(ErrorCode errorCode)
	{
		switch (errorCode)
		{
                case ErrorCode.BadVolume:
                    return ERR_INVALID_TRADE_VOLUME; 
                case ErrorCode.NoMoney:
                    return ERR_NOT_ENOUGH_MONEY;
                case ErrorCode.MarketClosed:
                    return ERR_MARKET_CLOSED;
                case ErrorCode.Disconnected:
                    return ERR_NO_CONNECTION;
                case ErrorCode.Timeout:
                    return ERR_TRADE_TIMEOUT;
                default:
                    return ERR_COMMON_ERROR;
		}
	}