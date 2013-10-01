class DesiredTrade
{
    public bool IsPosition { get; set; }
    public double? TakeProfit { get; set; }
    public double? StopLoss { get; set; }
}

Mq4Double OrderSend(Mq4String symbol, int cmd, Mq4Double volume, Mq4Double price, Mq4Double slippage, Mq4Double stoploss, 
    Mq4Double takeprofit, string comment = null, int magic = 0, int expiration = 0, int arrow_color = CLR_NONE)
{
    var symbolObject = GetSymbol(symbol);
    if (comment == null)
        comment = string.Empty;

    switch (cmd)
    {
        case OP_BUY:
        case OP_SELL:
            var tradeType = cmd == OP_BUY ? TradeType.Buy : TradeType.Sell;
            var volumeInMoney = volume * 100000;

            var request = new MarketOrderRequest(tradeType, volumeInMoney);            
            request.Label = magic.ToString();
            request.Symbol = symbolObject;
            Trade.Send(request);

            _desiredTrade = new DesiredTrade 
            { 
                IsPosition = true, 
                StopLoss = stoploss == 0 ? (double?)null : (double)stoploss, 
                TakeProfit = takeprofit == 0 ? (double?)null : (double)takeprofit,
            };
            _mq4Finished.Set();     
            _mq4Start.WaitOne();

            return GetTicket(_lastOpenedPosition);
        default:
            throw new Exception("Not supported by converter");            
    }

    return 0;
}