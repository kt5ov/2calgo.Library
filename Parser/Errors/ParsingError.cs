namespace _2calgo.Parser.Errors
{
    public class ParsingError
    {
        public ErrorType ErrorType { get; private set; }
        public string Message { get; private set; }
        public string Details { get; private set; }

        public ParsingError(ErrorType errorType, string error, string details)
        {
            ErrorType = errorType;
            Message = error;
            Details = details;
        }
    }
}