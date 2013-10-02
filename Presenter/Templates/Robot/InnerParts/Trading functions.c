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
    var volumeInMoney = volume * 100000;
    var label = magic.ToString();

    switch (cmd)
    {
        case OP_BUY:
        case OP_SELL:
            {
                var tradeType = cmd == OP_BUY ? TradeType.Buy : TradeType.Sell;
                var slippageInPrice = symbolObject.PointSize * slippage;
                var slippageInPips = (int)Math.Round((double)slippageInPrice / symbolObject.PipSize);

                var request = new MarketOrderRequest(tradeType, volumeInMoney);            
                request.Label = label;
                request.Symbol = symbolObject;
                request.SlippagePips = slippageInPips;
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
            }
        case OP_BUYLIMIT:
        case OP_SELLLIMIT:
        case OP_BUYSTOP:
        case OP_SELLSTOP:
            {
                var tradeType = cmd == OP_BUYLIMIT || cmd == OP_BUYSTOP ? TradeType.Buy : TradeType.Sell;
                PendingOrderRequest request;
                if (cmd == OP_BUYLIMIT || cmd == OP_SELLLIMIT)
                    request = new LimitOrderRequest(tradeType, volumeInMoney, price);
                else
                    request = new StopOrderRequest(tradeType, volumeInMoney, price);
                request.Label = label;
                request.Symbol = symbolObject;
                request.StopLoss = stoploss;
                request.TakeProfit = takeprofit;
                request.Expiration = Mq4TimeSeries.ToDateTime(expiration);

                _desiredTrade = new DesiredTrade 
                { 
                    IsPosition = false, 
                };

                Trade.Send(request);

                _mq4Finished.Set();     
                _mq4Start.WaitOne();

                return GetTicket(_lastPlacedOrder);
            }
        default:
            throw new Exception("Not supported by converter");            
    }

    return 0;
}