Mq4Double OrderSend(Mq4String symbol, int cmd, Mq4Double volume, Mq4Double price, Mq4Double slippagePoints, Mq4Double stoploss, 
    Mq4Double takeprofit, Mq4String comment = null, Mq4Double? magic = null, int expiration = 0, int arrow_color = CLR_NONE)
{
    _lastError = ERR_NO_ERROR;
    if (magic == null)
        magic = 0;
    var symbolObject = GetSymbol(symbol);
    var volumeInUnits = symbolObject.ToUnitsVolume(volume);;    
    var label = magic.Value.ToString();

    switch (cmd)
    {
        case OP_BUY:
        case OP_SELL:
            {
                var tradeType = cmd == OP_BUY ? TradeType.Buy : TradeType.Sell;
                var slippageInPrice = symbolObject.TickSize * slippagePoints;
                var slippageInPips = (int)Math.Round((double)slippageInPrice / symbolObject.PipSize);
                double? stopLossPips = null;
                if (stoploss != 0)
                    stopLossPips = tradeType == TradeType.Buy ? (symbolObject.Ask - stoploss) / symbolObject.PipSize : (stoploss - symbolObject.Bid) / symbolObject.PipSize;
                double? takeProfitPips = null;
                if (takeprofit != 0)
                    takeProfitPips = tradeType == TradeType.Buy ? (takeprofit - symbolObject.Ask) / symbolObject.PipSize : (symbolObject.Bid - takeprofit) / symbolObject.PipSize;

                var marketOrderResult = ExecuteMarketOrder(tradeType, symbolObject, volumeInUnits, label, stopLossPips, takeProfitPips, slippageInPips, comment);
				if (marketOrderResult.IsSuccessful)
					return GetTicket(marketOrderResult.Position);
				else
				{
					_lastError = ToMq4ErrorCode(marketOrderResult.Error.Value);
					return -1;
				}
            }
        case OP_BUYLIMIT:
        case OP_SELLLIMIT:
        case OP_BUYSTOP:
        case OP_SELLSTOP:
            {
                var tradeType = cmd == OP_BUYLIMIT || cmd == OP_BUYSTOP ? TradeType.Buy : TradeType.Sell;

                double? stopLossPips = null;
                if (stoploss != 0)
                    stopLossPips = tradeType == TradeType.Buy ? (price - stoploss) / symbolObject.PipSize : (stoploss - price) / symbolObject.PipSize;
                double? takeProfitPips = null;
                if (takeprofit != 0)
                    takeProfitPips = tradeType == TradeType.Buy ? (takeprofit - price) / symbolObject.PipSize : (price - takeprofit) / symbolObject.PipSize;

				TradeResult placeOrderResult;
                if (cmd == OP_BUYLIMIT || cmd == OP_SELLLIMIT)
                    placeOrderResult = PlaceLimitOrder(tradeType, symbolObject, volumeInUnits, price, label, stopLossPips, takeProfitPips, expiration.ToNullableDateTime(), comment);
                else
                    placeOrderResult = PlaceStopOrder(tradeType, symbolObject, volumeInUnits, price, label, stopLossPips, takeProfitPips, expiration.ToNullableDateTime(), comment);

				if (placeOrderResult.IsSuccessful)
					return GetTicket(placeOrderResult.PendingOrder);
				else
				{
					_lastError = ToMq4ErrorCode(placeOrderResult.Error.Value);
					return -1;
				}
            }
        default:
            throw new Exception("Not supported by converter");            
    }

    return 0;
}

[Conditional("OrderClose")]
Mq4Double OrderClose(int ticket, double lots, double price, int slippagePoints, int Color = CLR_NONE)
{
    _lastError = ERR_NO_ERROR;

    var position = GetOrderByTicket(ticket) as Position;
    if (position == null)
    {
        _lastError = ERR_INVALID_TICKET;
        return false;
    }
    var symbolObject = MarketData.GetSymbol(position.SymbolCode);
    
    var volumeInUnits = symbolObject.ToUnitsVolume(lots);    
    ClosePosition(position, volumeInUnits);    

	if (!LastResult.IsSuccessful)
		_lastError = ToMq4ErrorCode(LastResult.Error.Value);

    return LastResult.IsSuccessful;
}

[Conditional("OrderModify")]
Mq4Double OrderModify(int ticket, double price, double stoploss, double takeprofit, int expiration, int arrow_color=CLR_NONE)
{
    _lastError = ERR_NO_ERROR;

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
        ModifyPosition(position, stoploss.ToNullableDouble(), takeprofit.ToNullableDouble());
		if (!LastResult.IsSuccessful)
			_lastError = ToMq4ErrorCode(LastResult.Error.Value);

        return LastResult.IsSuccessful;
    }
    
    var pendingOrder = (PendingOrder)order;
    var expirationTime = expiration.ToNullableDateTime();
    ModifyPendingOrder(pendingOrder, price, stoploss.ToNullableDouble(), takeprofit.ToNullableDouble(), expirationTime);

	if (!LastResult.IsSuccessful)
		_lastError = ToMq4ErrorCode(LastResult.Error.Value);

    return LastResult.IsSuccessful;
}

[Conditional("OrderDelete")]
bool OrderDelete(int ticket, int Color = CLR_NONE)
{
    _lastError = ERR_NO_ERROR;
    
    var pendingOrder = GetOrderByTicket(ticket) as PendingOrder;
    if (pendingOrder == null)
        return false;

    CancelPendingOrder(pendingOrder);

	if (!LastResult.IsSuccessful)
		_lastError = ToMq4ErrorCode(LastResult.Error.Value);
    
    return LastResult.IsSuccessful;
}