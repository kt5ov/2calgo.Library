namespace _2calgo.Model
{
    public class MethodCall
    {
        public string MethodName { get; set; }
        public string[] Parameters { get; set; }
        public string OriginalText { get; set; }

        public MethodCall(string methodName, string[] parameters, string originalText)
        {
            MethodName = methodName;
            Parameters = parameters;
            OriginalText = originalText;
        }
    }
}