namespace _2calgo.Parser.Errors
{
    internal class MethodError
    {
        public string MethodName { get; private set; }
        public ErrorType ErrorType { get; private set; }

        public MethodError(string methodName, ErrorType errorType)
        {
            MethodName = methodName;
            ErrorType = errorType;
        }
    }
}