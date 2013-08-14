	[Conditional("StringConcatenate")]
	string StringConcatenate(params object[] parameters)
	{
		var stringBuilder = new StringBuilder();
		foreach (var parameter in parameters)
		stringBuilder.Append(parameter.ToString());       
		return stringBuilder.ToString();
	}

	[Conditional("StringFind")]
	int StringFind(string text, string matched_text, int start = 0)
	{
		return text.IndexOf(matched_text, start);
	}

	[Conditional("StringLen")]
	int StringLen(string text)
	{
		return text.Length;
	}

	[Conditional("StringSubstr")]
	string StringSubstr(string text, int start, int length = 0)
	{	
		if (length == 0 || length > text.Length - start)
			return text.Substring(start, text.Length - start);

		return text.Substring(start, length);
	}

	[Conditional("StringTrimLeft")]
	string StringTrimLeft(string text)
	{
		return text.TrimStart();
	}

	[Conditional("StringTrimRight")]
	string StringTrimRight(string text)
	{
		return text.TrimEnd();
	}