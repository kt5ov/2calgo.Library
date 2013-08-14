    [Conditional("MarketInfo")]
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

	[Conditional("Bid", "MarketInfo")]
    Mq4Double Bid
    {
      get 
      {
        if (Symbol == null || double.IsNaN(Symbol.Bid))
          return 0;
        return Symbol.Bid;
      }
    }

	[Conditional("Ask", "MarketInfo")]
    Mq4Double Ask
    {
      get 
      {
        if (Symbol == null || double.IsNaN(Symbol.Ask))
          return 0;
        return Symbol.Ask;
      }
    }

	[Conditional("Comment")]
    void Comment(params object[] objects)
    {
        var text = string.Join("", objects.Select(o => o.ToString()));
        ChartObjects.DrawText("top left comment", text, StaticPosition.TopLeft);
    }

	[Conditional("Alert")]
    void Alert(params object[] objects)
    {
        var text = string.Join("", objects.Select(o => o.ToString()));      
        Print("Alert: " + text);
    }

	[Conditional("Sleep")]
    void Sleep(int milliseconds)
    {        
    }

	[Conditional("PlaySound")]
    void PlaySound(string fileName)
    {
        Notifications.PlaySound(fileName);
    }