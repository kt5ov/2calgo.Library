[Conditional("MathArccos")]
Mq4Double MathArccos(double x)
{
    return Math.Acos(x);
}

[Conditional("MathArcsin")]
Mq4Double MathArcsin(double x)
{
    return Math.Asin(x);
}

[Conditional("MathMax")]
Mq4Double MathMax(double value1, double value2) 
{
    return Math.Max(value1, value2);
}
        
[Conditional("MathMin")]
Mq4Double MathMin(double value1, double value2) 
{ 
    return Math.Min(value1, value2);
}

[Conditional("MathSqrt")]
Mq4Double MathSqrt(double x) 
{
    return Math.Sqrt(x);
}

[Conditional("MathAbs")]
Mq4Double MathAbs(double x) 
{
    return Math.Abs(x);
}

[Conditional("MathRound")]
Mq4Double MathRound(double x) 
{
    return Math.Round(x);
}
        
[Conditional("MathPow")]
Mq4Double MathPow(double @base, double exponent)
{
    return Math.Pow(@base, exponent);
}

[Conditional("MathCeil")]
Mq4Double MathCeil(double x)
{
    return Math.Ceiling(x);
}

[Conditional("MathFloor")]
Mq4Double MathFloor(double x)
{
    return Math.Floor(x);
}
    
[Conditional("MathLog")]
Mq4Double MathLog(double x)
{
    return Math.Log(x);
}
    
[Conditional("MathCos")]
Mq4Double MathCos(double x)
{
    return Math.Cos(x);
}

[Conditional("MathSin")]
Mq4Double MathSin(double x)
{
    return Math.Sin(x);
}

[Conditional("MathMod")]
Mq4Double MathMod(double value, double value2)
{
    return value % value2;
}

[Conditional("MathExp")]
Mq4Double MathExp(double x)
{
    return Math.Exp(x);
}

[Conditional("MathArctan")]
Mq4Double MathArctan(double x)
{
    return Math.Atan(x);
}
	
[Conditional("MathRand", "MathSrand")]
//{
Random _rnd = new Random();

Mq4Double MathRand()
{		
	return _rnd.Next(32768);
}

void MathSrand(int seed)
{		
	if (seed == 1)
	{
		_rnd = new Random();
	}
	else
	{
		_rnd = new Random(seed);
	}
}
//}

[Conditional("MathTan")]
Mq4Double MathTan(Mq4Double x)
{
	return Math.Tan(x);
}