using System.Text.RegularExpressions;

namespace _2calgo.Parser.CodeAdapter
{
    public static class FunctionsRenaming
    {
         public static string RenameFunctions(this string code)
         {
             var functions = FunctionsParser.Parse(code);
             foreach (var function in functions)
             {
                 if (function.Name == "start" || function.Name == "init")
                     continue;

                 var regex = new Regex(@"(?<!\w)" + function.Name + @"\s*\(");
                 code = regex.Replace(code, function.Name + "Func(");
             }
             return code;
         }
    }
}