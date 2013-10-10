[Conditional("AccountFreeMargin")]
Mq4Double AccountFreeMargin()
{
    return Account.FreeMargin;
}

[Conditional("AccountBalance")]
Mq4Double AccountBalance()
{
    return Account.Balance;
}

[Conditional("AccountMargin")]
Mq4Double AccountMargin()
{
    return Account.Margin;
}

[Conditional("AccountEquity")]
Mq4Double AccountEquity()
{
	return Account.Equity;
}


