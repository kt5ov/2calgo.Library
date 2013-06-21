namespace _2calgo.Parser.CodeAdapter
{
    public static class Mq4Properties
    {
         public static string RemoveMq4Properies(this string code)
         {
             code = code.Replace("#property", "//#property");
             return code.RemoveComments();
         }
    }
}