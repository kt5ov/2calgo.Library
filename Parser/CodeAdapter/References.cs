using System.Linq;
using _2calgo.Model;

namespace _2calgo.Parser.CodeAdapter
{
    public static class References
    {
         public static string AddRefModifiers(this string code)
         {
             var functions = FunctionsParser.Parse(code);

             foreach (var function in functions
                        .ToArray()
                        .Where(f => f.Parameters.Any(p => p.ByReference)))
             {
                 var f = function;

                 foreach (var methodCall in MethodCallsParser.Parse(code)
                                                             .Where(call => call.MethodName == f.Name)
                                                             .ToArray())
                 {
                     foreach (var parameter in f.Parameters.Where(p => p.ByReference))
                     {
                         var parameterValue = methodCall.Parameters[parameter.Index];
                         if (parameter.Type != "IMq4DoubleArray")
                            methodCall.Parameters[parameter.Index] = "ref " + parameterValue;
                     }
                     code = code.Replace(methodCall.OriginalText, methodCall.ToCSharpCode());
                 }
             }

             return code;
         }
    }
}