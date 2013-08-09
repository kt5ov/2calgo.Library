namespace _2calgo.Model
{
    public class MethodCall
    {
        public string MethodName { get; private set; }
        public string[] Parameters { get; private set; }
        public string OriginalText { get; private set; }

        public MethodCall(string methodName, string[] parameters, string originalText)
        {
            MethodName = methodName;
            Parameters = parameters;
            OriginalText = originalText;
        }
    }
}