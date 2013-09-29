[Conditional("StringConcatenate")]
Mq4String StringConcatenate(params object[] parameters)
{
	var stringBuilder = new StringBuilder();
	foreach (var parameter in parameters)
	stringBuilder.Append(parameter.ToString());       
	return stringBuilder.ToString();
}

[Conditional("StringFind")]
int StringFind(Mq4String text, string matched_text, int start = 0)
{
	return ((string)text).IndexOf(matched_text, start);
}

[Conditional("StringLen")]
int StringLen(Mq4String text)
{
	return ((string)text).Length;
}

[Conditional("StringSubstr")]
Mq4String StringSubstr(Mq4String text, int start, int length = 0)
{	
	if (length == 0 || length > ((string)text).Length - start)
		return ((string)text).Substring(start, ((string)text).Length - start);

	return ((string)text).Substring(start, length);
}

[Conditional("StringTrimLeft")]
Mq4String StringTrimLeft(Mq4String text)
{
	return ((string)text).TrimStart();
}

[Conditional("StringTrimRight")]
Mq4String StringTrimRight(Mq4String text)
{
	return ((string)text).TrimEnd();
}
	
[Conditional("StringSetChar")]
//{
Mq4String StringSetChar(Mq4String text, int pos, char value)
{
	return ((string)text)
			.Remove(pos, 1)
			.Insert(pos, value.ToString());
}	
Mq4String StringSetChar(Mq4String text, int pos, int value)
{
	return StringSetChar(text, pos, (char)value);
}
//}
	
[Conditional("StringGetChar")]
char StringGetChar(Mq4String text, int pos)
{
	return ((string)text)[pos];
}
