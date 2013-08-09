using _2calgo.Model;

namespace _2calgo.Parser.CodeAdapter
{
    public static class MethodCallsPresenter
    {
         public static string ToCSharpCode(this MethodCall methodCall)
         {
             return string.Format("{0}({1})", methodCall.MethodName, string.Join(", ", methodCall.Parameters));
         }
    }
}