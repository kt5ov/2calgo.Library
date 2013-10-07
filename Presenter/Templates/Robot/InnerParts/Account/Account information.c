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


