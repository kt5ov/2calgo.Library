const int MODE_TRADES = 0;
const int SELECT_BY_POS = 0;

private int GetTicket(Position position)
{
	return position.Id + (int)1e8;
}

private int GetTicket(PendingOrder pendingOrder)
{
	return pendingOrder.Id + (int)1e8;
}

private int GetMagicNumber(string label)
{
	int magicNumber;
	if (int.TryParse(label, out magicNumber))
		return magicNumber;
	return 0;
}

private int GetMagicNumber(Position position)
{
	return GetMagicNumber(position.Label);
}

private int GetMagicNumber(PendingOrder pendingOrder)
{
	return GetMagicNumber(pendingOrder.Label);
}

private int GetMagicNumber(object order)
{
	if (order is Position)
		return GetMagicNumber((Position)order);
	if (order is PendingOrder)
		return GetMagicNumber((PendingOrder)order);
	return 0;
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

	double volume;

	var currentPosition = _currentOrder as Position;
	if (currentPosition != null)
		volume = currentPosition.Volume;
	else
		volume = ((PendingOrder)_currentOrder).Volume;

	return volume / 100000;
}

[Conditional("OrderMagicNumber")]
Mq4Double OrderMagicNumber()
{
	if (_currentOrder == null)
		return 0;

	return GetMagicNumber(_currentOrder);
}
