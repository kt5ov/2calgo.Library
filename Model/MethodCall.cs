namespace _2calgo.Model
{
    public class MethodCall
    {
        public string MethodName { get; private set; }
        public string[] Parameters { get; private set; }

        public MethodCall(string methodName, string[] parameters)
        {
            MethodName = methodName;
            Parameters = parameters;
        }
    }
}