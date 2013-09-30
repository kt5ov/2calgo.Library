const int MODE_TRADES = 0;
const int SELECT_BY_POS = 0;


[Conditional("OrdersTotal")]
int OrdersTotal()
{
	return Account.Positions.Count + Account.PendingOrders.Count;
}

object _currentOrder;

[Conditional("OrderSelect")]
bool OrderSelect(int index, int select, int pool = MODE_TRADES)
{
	_currentOrder = null;

	if (select == SELECT_BY_POS)
	{

	}

	return true;
}