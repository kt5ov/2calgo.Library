const int MODE_TRADES = 0;
const int SELECT_BY_POS = 0;
const int SELECT_BY_TICKET = 1;

T GetPropertyValue<T>(Func<Position, T> getFromPosition, Func<PendingOrder, T> getFromPendingOrder)
{
	if (_currentOrder == null)
		return default(T);

	return GetPropertyValue<T>(_currentOrder, getFromPosition, getFromPendingOrder);
}

T GetPropertyValue<T>(object obj, Func<Position, T> getFromPosition, Func<PendingOrder, T> getFromPendingOrder)
{
	if (obj is Position)
		return getFromPosition((Position) obj);
	return getFromPendingOrder((PendingOrder) obj);
}

private Mq4Double GetTicket(object trade)
{
	return new Mq4Double(GetPropertyValue<int>(trade, _ => _.Id, _ => _.Id) + (int)1e8);
}

[Conditional("OrderTicket")]
Mq4Double OrderTicket()
{
	if (_currentOrder == null)
		return 0;

	return GetTicket(_currentOrder);
}

private int GetMagicNumber(string label)
{			
	var magicNumberStr = label;
	if (string.IsNullOrEmpty(magicNumberStr))
		return 0;

	var sharpIndex = magicNumberStr.IndexOf("#");
	if (sharpIndex != -1)
		magicNumberStr = magicNumberStr.Substring(0, sharpIndex);

	int magicNumber;
	if (int.TryParse(magicNumberStr, out magicNumber))
		return magicNumber;

	return 0;
}

private string GetComment(string label)
{
	if (string.IsNullOrEmpty(label))
		return string.Empty;

	var sharpIndex = label.IndexOf("#");
	if (sharpIndex == -1)
		return string.Empty;

	return label.Substring(sharpIndex, label.Length - sharpIndex);
}

private int GetMagicNumber(object order)
{	
	var label = GetPropertyValue<string>(order, _ => _.Label, _ => _.Label);
	return GetMagicNumber(label);
}

[Conditional("OrderMagicNumber")]
Mq4Double OrderMagicNumber()
{
	if (_currentOrder == null)
		return 0;

	return GetMagicNumber(_currentOrder);
}

[Conditional("OrderComment")]
Mq4String OrderComment()
{
	if (_currentOrder == null)
		return string.Empty;

	var label = GetPropertyValue<string>(_currentOrder, _ => _.Label, _ => _.Label);
	return GetComment(label);
}

[Conditional("OrdersTotal")]
Mq4Double OrdersTotal()
{
	return Account.Positions.Count + Account.PendingOrders.Count;
}

object _currentOrder;

[Conditional("OrderSelect")]
bool OrderSelect(int index, int select, int pool = MODE_TRADES)
{
	_currentOrder = null;

	var allOrders = Account.Positions.OfType<object>()
							.Concat(Account.PendingOrders.OfType<object>())
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

	return false;
}

double GetLots(object order)
{
	var volume = GetPropertyValue<long>(order, _ => _.Volume,  _ => _.Volume);

	return volume / 100000;
}

object GetOrderByTicket(int ticket)
{
	var allOrders = Account.Positions.OfType<object>()
							.Concat(Account.PendingOrders.OfType<object>())
							.ToArray();

	return allOrders.FirstOrDefault(_ => GetTicket(_) == ticket);
}

[Conditional("OrderLots")]
Mq4Double OrderLots()
{
	if (_currentOrder == null)
		return 0;

	return GetLots(_currentOrder);
}

[Conditional("OrderType")]
Mq4Double OrderType()
{
	if (_currentOrder == null)
		return 0;

	var position = _currentOrder as Position;
	if (position != null)
	{
		return position.TradeType == TradeType.Buy ? OP_BUY : OP_SELL;
	}
	var pendingOrder = (PendingOrder)_currentOrder;
	if (pendingOrder.OrderType == PendingOrderType.Limit)
		return pendingOrder.TradeType == TradeType.Buy ? OP_BUYLIMIT : OP_SELLLIMIT;
	return pendingOrder.TradeType == TradeType.Buy ? OP_BUYSTOP : OP_SELLSTOP;
}

[Conditional("OrderSymbol")]
Mq4String OrderSymbol()
{
	return GetPropertyValue<string>(_ => _.SymbolCode, _ => _.SymbolCode);
}

double GetOpenPrice(object order)
{
	return GetPropertyValue<double>(order, _ => _.EntryPrice, _ => _.TargetPrice);
}

[Conditional("OrderOpenPrice")]
Mq4Double OrderOpenPrice()
{
	if (_currentOrder == null)
		return 0;
	return GetOpenPrice(_currentOrder);
}

private double GetStopLoss(object order)
{
	var nullableValue = GetPropertyValue<double?>(order, _ => _.StopLoss, _ => _.StopLoss);
	return nullableValue ?? 0;
}

private double GetTakeProfit(object order)
{
	var nullableValue = GetPropertyValue<double?>(order, _ => _.TakeProfit, _ => _.TakeProfit);
	return nullableValue ?? 0;
}

[Conditional("OrderStopLoss")]
Mq4Double OrderStopLoss()
{	
	if (_currentOrder == null)
		return 0;
	return GetStopLoss(_currentOrder);
}

[Conditional("OrderTakeProfit")]
Mq4Double OrderTakeProfit()
{
	if (_currentOrder == null)
		return 0;
	return GetTakeProfit(_currentOrder);
}

[Conditional("OrderProfit")]
Mq4Double OrderProfit()
{
	var position = _currentOrder as Position;
	if (position == null)
		return 0;
	return position.NetProfit;
}

[Conditional("OrderOpenTime")]
Mq4Double OrderOpenTime()
{
	var position = _currentOrder as Position;
	if (position == null)
		return 0;

	return Mq4TimeSeries.ToInteger(position.EntryTime);
}

[Conditional("OrderSwap")]
Mq4Double OrderSwap()
{
	var position = _currentOrder as Position;
	if (position == null)
		return 0;

	return position.Swap;
}