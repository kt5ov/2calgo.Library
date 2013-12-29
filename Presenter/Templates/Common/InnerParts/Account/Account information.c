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

[Conditional("AccountName")]
Mq4String AccountName()
{
	return "Unknown";
}

[Conditional("AccountCredit")]
Mq4Double AccountCredit()
{
	return 0;
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

[Conditional("AccountFreeMarginMode")]
Mq4Double AccountFreeMarginMode()
{
	return 1;
}

[Conditional("AccountFreeMarginCheck")]
Mq4Double AccountFreeMarginCheck(Mq4String symbol, Mq4Double cmd, Mq4Double volume)
{
	_lastError = ERR_NO_ERROR;
	var result = Account.FreeMargin - volume * MarketInfo(symbol, MODE_MARGINREQUIRED);
	if (result <= 0)
		_lastError = ERR_NOT_ENOUGH_MONEY;

	return result;
}