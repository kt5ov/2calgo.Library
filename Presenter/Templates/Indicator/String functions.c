#region String functions

	string StringConcatenate(params object[] parameters)
	{
		var stringBuilder = new StringBuilder();
		foreach (var parameter in parameters)
		stringBuilder.Append(parameter.ToString());       
		return stringBuilder.ToString();
	}

	int StringFind(string text, string matched_text, int start = 0)
	{
		return text.IndexOf(matched_text, start);
	}

	int StringLen(string text)
	{
		return text.Length;
	}

	string StringSubstr(string text, int start, int length = 0)
	{	
		if (length == 0 || length > text.Length - start)
			return text.Substring(start, text.Length - start);

		return text.Substring(start, length);
	}

	string StringTrimLeft(string text)
	{
		return text.TrimStart();
	}

	string StringTrimRight(string text)
	{
		return text.TrimEnd();
	}

#endregion //String functions