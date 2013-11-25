struct Mq4Char
{
	char _char;
			
	public Mq4Char(byte code)
	{
		_char = Encoding.Unicode.GetString(new byte[]{code, 0})[0];	
	}

	public Mq4Char(char @char)
	{
		_char = @char;
	}

	public static implicit operator char(Mq4Char mq4Char)
	{
		return mq4Char._char;
	}
	
	public static implicit operator Mq4Char(int code)
	{
		return new Mq4Char((byte)code);
	}
	
	public static implicit operator Mq4Char(string str)
	{
		if (string.IsNullOrEmpty(str) || str.Length == 0)
			return new Mq4Char(' ');
		return new Mq4Char(str[0]);
	}
}