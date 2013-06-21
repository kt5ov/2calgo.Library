namespace _2calgo.Parser.CodeAdapter
{
    public static class Mq4Parameters
    {
         public static string RemoveMq4Parameters(this string code)
         {
             var str = ParametersParser.ParameterRegex.Replace(code, string.Empty);

             return ParametersParser.ParameterRegex.Replace(str, string.Empty);
         }
    }
}