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