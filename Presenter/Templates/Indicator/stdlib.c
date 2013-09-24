	[Conditional("ErrorDescription")]
    Mq4String ErrorDescription(int code)
	{
		string error_string;
		switch (code)
			{
				case 0:
				case 1:   return "no error";  
				case 2:   return "common error";                                              
				case 4203: return "unknown object type";                                      
				case 4204: return "no object name";                                           
				case 4205: return "object coordinates error";                                 
				default:   return "unknown error";
			}
		return error_string;
	}

	[Conditional("CompareDoubles")]
	bool CompareDoubles(double number1, double number2)
	{
		return Math.Abs(number1 - number2) < 0.00000001;
	}

