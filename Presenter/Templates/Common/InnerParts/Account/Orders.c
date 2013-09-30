[Conditional("OrdersTotal")]
int OrdersTotal()
{
	return Account.Positions.Count + Account.PendingOrders.Count;
}