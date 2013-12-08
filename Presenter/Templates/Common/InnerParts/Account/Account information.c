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

[Conditional("AccountCurrency")]
Mq4String AccountCurrency()
{
    return Account.Currency;
}

[Conditional("AccountServer")]
Mq4String AccountServer()
{
	return "Unknown Server";
}

[Conditional("AccountLeverage")]
Mq4Double AccountLeverage()
{
    return Account.Leverage;
}

[Conditional("AccountProfit")]
Mq4Double AccountProfit()
{
    return Positions.Sum(p => p.NetProfit);
}

[Conditional("AccountNumber")]
Mq4Double AccountNumber()
{
	return 0;
}

[Conditional("AccountCompany")]
Mq4String AccountCompany()
{
	return "Unknown";
}