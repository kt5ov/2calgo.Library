class DesiredTrade {}

class OpenPositionTrade : DesiredTrade
{
    public double? StopLoss { get; set; }
    public double? TakeProfit { get; set; }    
}

class CreatePendingOrderTrade : DesiredTrade { }

class ClosePositionTrade : DesiredTrade {}

class ProtectPositionTrade : DesiredTrade
{
    public double? StopLoss { get; set; }
    public double? TakeProfit { get; set; }    
}

class ModifyPendingOrderTrade : DesiredTrade
{
    public double? StopLoss { get; set; }
    public double? TakeProfit { get; set; }    
}

class CancelPendingOrderTrade : DesiredTrade 
{
    public PendingOrder PendingOrder { get; set; }
}

Mq4Double OrderSend(Mq4String symbol, int cmd, Mq4Double volume, Mq4Double price, Mq4Double slippage, Mq4Double stoploss, 
    Mq4Double takeprofit, string comment = null, int magic = 0, int expiration = 0, int arrow_color = CLR_NONE)
{
    var symbolObject = GetSymbol(symbol);
    if (comment == null)
        comment = string.Empty;
    var volumeInMoney = volume * 100000;    
    var label = magic.ToString() + (comment == string.Empty ? string.Empty : "#" + comment);

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

                _desiredTrade = new OpenPositionTrade 
                { 
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

                _desiredTrade = new CreatePendingOrderTrade();

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

[Conditional("OrderClose")]
Mq4Double OrderClose(int ticket, double lots, double price, int slippage, int Color = CLR_NONE)
{
    var position = GetOrderByTicket(ticket) as Position;
    if (position == null)
    {
        _lastError = ERR_INVALID_TICKET;
        return false;
    }
    if (GetLots(position) != lots)
        throw new Exception("Partial close isn't supported by cAlgo");

    _desiredTrade = new ClosePositionTrade();
    Trade.Close(position);
    
    _mq4Finished.Set();     
    _mq4Start.WaitOne();

    return true;
}

[Conditional("OrderModify")]
Mq4Double OrderModify(int ticket, double price, double stoploss, double takeprofit, int expiration, int arrow_color=CLR_NONE)
{
    var order = GetOrderByTicket(ticket);
    if (GetTakeProfit(order) == takeprofit && GetStopLoss(order) == stoploss
        && GetOpenPrice(order) == price)
    {
        _lastError = ERR_NO_RESULT;
        return false;
    }

    var position = order as Position;
    if (position != null)
    {
        _desiredTrade = new ProtectPositionTrade
        {
            StopLoss = stoploss.ToNullableDouble(),
            TakeProfit = takeprofit.ToNullableDouble(),
        };                
        _positionToProtect = position;
        Trade.ModifyPosition(position, stoploss.ToNullableDouble(), takeprofit.ToNullableDouble());

        _mq4Finished.Set();     
        _mq4Start.WaitOne();

        return true;
    }
    
    var pendingOrder = (PendingOrder)order;
    _desiredTrade = new ModifyPendingOrderTrade
    {
        StopLoss = stoploss.ToNullableDouble(),
        TakeProfit = takeprofit.ToNullableDouble(),
    };                
    _pendingOrderToModify = pendingOrder;
    Trade.ModifyPendingOrder(pendingOrder, stoploss.ToNullableDouble(), takeprofit.ToNullableDouble());

    _mq4Finished.Set();     
    _mq4Start.WaitOne();

    return true;
}

[Conditional("OrderDelete")]
bool OrderDelete(int ticket, int Color = CLR_NONE)
{
    var pendingOrder = GetOrderByTicket(ticket) as PendingOrder;
    if (pendingOrder == null)
        return false;

    _desiredTrade = new CancellPendingOrderTrade
    {
        PendingOrder = pendingOrder,
    };

    Trade.DeletePendingOrder(pendingOrder);
    
    _mq4Finished.Set();     
    _mq4Start.WaitOne();
}
