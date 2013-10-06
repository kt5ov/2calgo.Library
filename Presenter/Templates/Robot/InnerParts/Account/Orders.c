const int MODE_TRADES = 0;
const int SELECT_BY_POS = 0;

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
	return GetPropertyValue<int>(trade, _ => _.Id, _ => _.Id) + (int)1e8;
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

	if (select == SELECT_BY_POS)
	{
		if (index < 0 || index >= allOrders.Length)
			return false;

		_currentOrder = allOrders[index];
	}

	return true;
}

[Conditional("OrderLots")]
Mq4Double OrderLots()
{
	if (_currentOrder == null)
		return 0;

	var volume = GetPropertyValue<long>(_currentOrder, _ => _.Volume,  _ => _.Volume);

	return volume / 100000;
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

[Conditional("OrderOpenPrice")]
Mq4Double OrderOpenPrice()
{
	return GetPropertyValue<double>(_ => _.EntryPrice, _ => _.TargetPrice);
}

[Conditional("OrderStopLoss")]
Mq4Double OrderStopLoss()
{	
	var nullableValue = GetPropertyValue<double?>(_ => _.StopLoss, _ => _.StopLoss);
	return nullableValue ?? 0;
}

[Conditional("OrderTakeProfit")]
Mq4Double OrderTakeProfit()
{
	var nullableValue = GetPropertyValue<double?>(_ => _.TakeProfit, _ => _.TakeProfit);
	return nullableValue ?? 0;
}