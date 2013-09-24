using System.Collections.Generic;
using System.Linq;

namespace _2calgo.Parser.CodeAdapter
{
// ReSharper disable InconsistentNaming
    public static class ICustom
// ReSharper restore InconsistentNaming
    {
         public static string AddTypeParameterToICustom(this string code, out string[] customIndicators)
         {
             var resultIndicators = new List<string>();

             var methodCalls = MethodCallsParser.Parse(code)
                 .Where(mc => mc.MethodName == "iCustom")
                 .ToArray();

             foreach (var methodCall in methodCalls)
             {
                 var nameParameter = methodCall.Parameters[2];
                 var simplifiedName = AlgoNameProvider.GetSimplifiedName(nameParameter);

                 var stringToReplace = methodCall.OriginalText.Replace("iCustom", string.Format("iCustom<{0}>", simplifiedName));
                 code = code.Replace(methodCall.OriginalText, stringToReplace);
                 resultIndicators.Add(simplifiedName);
             }

             customIndicators = resultIndicators.Distinct().ToArray();
             return code;
         }
    }
}