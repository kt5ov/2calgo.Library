using System.Linq;

namespace _2calgo.Parser.CodeAdapter
{
    public static class References
    {
         public static string AddRefModifiers(this string code)
         {
             var functions = FunctionsParser.Parse(code);
             var methodCalls = MethodCallsParser.Parse(code).ToArray();

             foreach (var function in functions.ToArray())
             {
                 foreach (var referenceParameter in function.Parameters.Where(parameter => parameter.ByReference))
                 {
                     
                 }
             }

             return code;
         }
    }
}