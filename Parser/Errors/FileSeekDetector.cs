using System.Collections.Generic;
using System.Linq;

namespace _2calgo.Parser.Errors
{
    class FileSeekDetector : ErrorDetector
    {
        public override IEnumerable<ParsingError> FindErrors(string code)
        {
            var methodCalls = MethodCallsParser.Parse(code).ToArray();
            foreach (var call in methodCalls.Where(c => c.MethodName == "FileSeek"))
            {
                if (call.Parameters[1] != "0" || (call.Parameters[2] != "SEEK_END" && call.Parameters[2] != "2"))
                    yield return new ParsingError(ErrorType.NotSupportedError, "FileSeek", call.OriginalText);
            }
        }
    }
}