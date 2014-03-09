const int MODE_TRADES = 0;
const int MODE_HISTORY = 1;
const int SELECT_BY_POS = 0;
const int SELECT_BY_TICKET = 1;

T GetPropertyValue<T>(Func<Position, T> getFromPosition, Func<PendingOrder, T> getFromPendingOrder, Func<HistoricalTrade, T> getFromHistory)
{
	if (_currentOrder == null)
		return default(T);

	return GetPropertyValue<T>(_currentOrder, getFromPosition, getFromPendingOrder, getFromHistory);
}

T GetPropertyValue<T>(object obj, Func<Position, T> getFromPosition, Func<PendingOrder, T> getFromPendingOrder, Func<HistoricalTrade, T> getFromHistory)
{
	if (obj is Position)
		return getFromPosition((Position) obj);
	if (obj is PendingOrder)
		return getFromPendingOrder((PendingOrder) obj);

	return getFromHistory((HistoricalTrade) obj);
} 

private Mq4Double GetTicket(object trade)
{
	return new Mq4Double(GetPropertyValue<int>(trade, _ => _.Id, _ => _.Id, _ => _.ClosingDealId));
}

[Conditional("OrderTicket", "OrderPrint")]
Mq4Double OrderTicket()
{
	if (_currentOrder == null)
		return 0;

	return GetTicket(_currentOrder);
}

private int GetMagicNumber(string label)
{			
	int magicNumber;
	if (int.TryParse(label, out magicNumber))
		return magicNumber;

	return 0;
}

private int GetMagicNumber(object order)
{	
	var label = GetPropertyValue<string>(order, _ => _.Label, _ => _.Label, _ => _.Label);
	return GetMagicNumber(label);
}

[Conditional("OrderMagicNumber", "OrderPrint")]
Mq4Double OrderMagicNumber()
{
	if (_currentOrder == null)
		return 0;

	return GetMagicNumber(_currentOrder);
}

[Conditional("OrderComment", "OrderPrint")]
Mq4String OrderComment()
{
	if (_currentOrder == null)
		return string.Empty;

	return GetPropertyValue<string>(_currentOrder, _ => _.Comment, _ => _.Comment, _ => _.Comment);
}

[Conditional("OrdersTotal")]
Mq4Double OrdersTotal()
{
	return Positions.Count + PendingOrders.Count;
}

[Conditional("HistoryTotal")]
Mq4Double HistoryTotal()
{
	return History.Count;
}

[Conditional("OrdersHistoryTotal")]
Mq4Double OrdersHistoryTotal()
{
	return History.Count;
}

object _currentOrder;

[Conditional("OrderSelect")]
bool OrderSelect(int index, int select, int pool = MODE_TRADES)
{
	_currentOrder = null;

	if (pool == MODE_TRADES)
	{
		var allOrders = Positions.OfType<object>()
								.Concat(PendingOrders.OfType<object>())
								.ToArray();

		switch (select)
		{
			case SELECT_BY_POS:	
				if (index < 0 || index >= allOrders.Length)
					return false;

				_currentOrder = allOrders[index];
				return true;
			case SELECT_BY_TICKET:
				_currentOrder = GetOrderByTicket(index);
				return _currentOrder != null;
		}
	}	
	if (pool == MODE_HISTORY)
	{
		switch (select)
		{
			case SELECT_BY_POS:	
				if (index < 0 || index >= History.Count)
					return false;

				_currentOrder = History[index];
				return true;
			case SELECT_BY_TICKET:
				_currentOrder = History.FindLast(index.ToString());
				return _currentOrder != null;
		}
	}

	return false;
}

double GetLots(object order)
{
	var volume = GetPropertyValue<long>(order, _ => _.Volume,  _ => _.Volume, _ => _.Volume);
	var symbolCode = GetPropertyValue<string>(order, _ => _.SymbolCode, _ => _.SymbolCode, _ => _.SymbolCode);
	var symbolObject = MarketData.GetSymbol(symbolCode);
	
	return symbolObject.ToLotsVolume(volume);
}

object GetOrderByTicket(int ticket)
{
	var allOrders = Positions.OfType<object>()
							.Concat(PendingOrders.OfType<object>())
							.ToArray();

	return allOrders.FirstOrDefault(_ => GetTicket(_) == ticket);
}

[Conditional("OrderLots", "OrderPrint")]
Mq4Double OrderLots()
{
	if (_currentOrder == null)
		return 0;

	return GetLots(_currentOrder);
}

[Conditional("OrderType", "OrderPrint")]
Mq4Double OrderType()
{
	if (_currentOrder == null)
		return 0;

	var position = _currentOrder as Position;
	if (position != null)
	{
		return position.TradeType == TradeType.Buy ? OP_BUY : OP_SELL;
	}
	var pendingOrder = _currentOrder as PendingOrder;
	if (pendingOrder != null)
	{
		if (pendingOrder.OrderType == PendingOrderType.Limit)
			return pendingOrder.TradeType == TradeType.Buy ? OP_BUYLIMIT : OP_SELLLIMIT;
		return pendingOrder.TradeType == TradeType.Buy ? OP_BUYSTOP : OP_SELLSTOP;
	}

	var historicalTrade = (HistoricalTrade)_currentOrder;

	return historicalTrade.TradeType == TradeType.Buy ? OP_BUY : OP_SELL;
}

[Conditional("OrderSymbol")]
Mq4String OrderSymbol()
{
	return GetPropertyValue<string>(_ => _.SymbolCode, _ => _.SymbolCode, _ => _.SymbolCode);
}

double GetOpenPrice(object order)
{
	return GetPropertyValue<double>(order, _ => _.EntryPrice, _ => _.TargetPrice, _ => _.EntryPrice);
}

[Conditional("OrderOpenPrice", "OrderPrint")]
Mq4Double OrderOpenPrice()
{
	if (_currentOrder == null)
		return 0;

	return GetOpenPrice(_currentOrder);
}

[Conditional("OrderClosePrice")]
Mq4Double OrderClosePrice()
{
	var historicalTrade = _currentOrder as HistoricalTrade;
	if (historicalTrade != null)
		return historicalTrade.ClosingPrice;

	return 0;
}

[Conditional("OrderCloseTime")]
Mq4Double OrderCloseTime()
{
	var historicalTrade = _currentOrder as HistoricalTrade;
	if (historicalTrade != null)
		return Mq4TimeSeries.ToInteger(historicalTrade.ClosingTime);

	return 0;
}

private double GetStopLoss(object order)
{
	var nullableValue = GetPropertyValue<double?>(order, _ => _.StopLoss, _ => _.StopLoss, _ => 0);
	return nullableValue ?? 0;
}

private double GetTakeProfit(object order)
{
	var nullableValue = GetPropertyValue<double?>(order, _ => _.TakeProfit, _ => _.TakeProfit, _ => 0);
	return nullableValue ?? 0;
}

[Conditional("OrderStopLoss", "OrderPrint")]
Mq4Double OrderStopLoss()
{	
	if (_currentOrder == null)
		return 0;
	return GetStopLoss(_currentOrder);
}

[Conditional("OrderTakeProfit", "OrderPrint")]
Mq4Double OrderTakeProfit()
{
	if (_currentOrder == null)
		return 0;
	return GetTakeProfit(_currentOrder);
}

[Conditional("OrderProfit", "OrderPrint")]
Mq4Double OrderProfit()
{
	var position = _currentOrder as Position;
	if (position != null)
		return position.NetProfit;
	
	var historicalTrade = _currentOrder as HistoricalTrade;
	if (historicalTrade != null)
		return historicalTrade.NetProfit;
	
	return 0;
}

[Conditional("OrderOpenTime", "OrderPrint")]
Mq4Double OrderOpenTime()
{
	var position = _currentOrder as Position;
	if (position != null)
		return Mq4TimeSeries.ToInteger(position.EntryTime);

	var historicalTrade = _currentOrder as HistoricalTrade;
	if (historicalTrade != null)
		return Mq4TimeSeries.ToInteger(historicalTrade.EntryTime);

	return 0;
}

[Conditional("OrderExpiration", "OrderPrint")]
Mq4Double OrderExpiration()
{
	var pendingOrder = _currentOrder as PendingOrder;
	if (pendingOrder == null || pendingOrder.ExpirationTime == null)
		return 0;

	return Mq4TimeSeries.ToInteger(pendingOrder.ExpirationTime.Value);
}

[Conditional("OrderSwap", "OrderPrint")]
Mq4Double OrderSwap()
{
	var position = _currentOrder as Position;
	if (position != null)
		return position.Swap;

	var historicalTrade = _currentOrder as HistoricalTrade;
	if (historicalTrade != null)
		return historicalTrade.Swap;

	return 0;
}

[Conditional("OrderCommission", "OrderPrint")]
Mq4Double OrderCommission()
{
	var position = _currentOrder as Position;
	if (position != null)
		return position.Commissions;

	var historicalTrade = _currentOrder as HistoricalTrade;
	if (historicalTrade != null)
		return historicalTrade.Commissions;

	return 0;
}

[Conditional("OrderPrint")]
private string OrderTypeString()
{
	switch(OrderType())
	{
		case OP_BUY:
			return "buy";
		case OP_SELL:
			return "sell";
		case OP_BUYLIMIT:
			return "buy limit";
		case OP_SELLLIMIT:
			return "sell limit";
		case OP_BUYSTOP:
			return "buy stop";
		case OP_SELLSTOP:
			return "sell stop";
	}
	return "0";
}

[Conditional("OrderPrint")]
void OrderPrint()
{
	var values = new string[]
	{
		OrderTicket().ToString(),
		Mq4TimeSeries.ToDateTime(OrderOpenTime()).ToString(),
		OrderTypeString(),
		OrderLots().ToString(),
		OrderOpenPrice().ToString(),
		OrderStopLoss().ToString(),
		OrderTakeProfit().ToString(),
		"0",
		"0",
		OrderCommission().ToString(),
		OrderSwap().ToString(),
		OrderProfit().ToString(),
		OrderComment(),
		OrderMagicNumber().ToString(),
		OrderExpiration() == 0 ? "0" : Mq4TimeSeries.ToDateTime(OrderExpiration()).ToString(),
	};
	Print(string.Join("; ", values));
}