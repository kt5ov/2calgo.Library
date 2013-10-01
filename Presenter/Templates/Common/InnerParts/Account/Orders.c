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
