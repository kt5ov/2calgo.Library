static class ConvertExtensions
{
    public static double? ToNullableDouble(this double protection)
    {
        if (protection == 0)
            return null;
        return protection;
    }

    public static DateTime? ToNullableDateTime(this int time)
    {
        if (time == 0)
            return null;

        return Mq4TimeSeries.ToDateTime(time);
    }       

    public static long ToUnitsVolume(this Symbol symbol, double lots) 
    {        
        return symbol.NormalizeVolume(symbol.ToNotNormalizedUnitsVolume(lots));
    }

    public static double ToNotNormalizedUnitsVolume(this Symbol symbol, double lots) 
    {
        if (symbol.Code.Contains("XAU") || symbol.Code.Contains("XAG"))
            return 100 * lots;
            
        return 100000 * lots;
    }

    public static double ToLotsVolume(this Symbol symbol, long volume) 
    {
        if (symbol.Code.Contains("XAU") || symbol.Code.Contains("XAG"))
            return volume * 1.0 / 100;
            
        return volume * 1.0 / 100000;
    }
}