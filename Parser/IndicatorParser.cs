using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Threading;
using _2calgo.Model;
using _2calgo.Parser.CodeAdapter;
using _2calgo.Parser.Errors;

namespace _2calgo.Parser
{
    public class IndicatorParser
    {
        public IndicatorParsingResult Parse(string code)
        {
            Thread.CurrentThread.CurrentCulture = CultureInfo.InvariantCulture;
            var parsingErrors = new ParsingErrors();

            var indicator = new Indicator();

            code = code
                .RemoveComments()
                .HandleParsingErrors(parsingErrors)
                .ReplaceDefines()
                .ReplaceCSharpKeyWords()
                .RemoveDotsFromNames()
                .RemoveDollars()
                .ReplaceDateTimeToInt()
                .ReplaceColorToInt();

            HandleProperties(code, indicator);
            HandleParameters(code, indicator);
            HandleFunctions(code, indicator, parsingErrors);
            HandleFields(code, indicator);

            indicator.Code.ExtractStaticVariablesToFields();
            indicator.Code.ReplaceSimpleTypesToMq4Double();
            indicator.Code.AddMq4InitFunctionIfDoesNotExist();
            
            return new IndicatorParsingResult(indicator, parsingErrors.Errors);
        }
        
        private void HandleFunctions(string code, Indicator indicator, ParsingErrors parsingErrors)
        {
            var withoutProperties = code.RemoveMq4Properies();
            var mq4Functions = FunctionsParser.Parse(withoutProperties).ToArray();
            indicator.Code.Functions = mq4Functions.ToList();
            HandleInit(indicator, mq4Functions, parsingErrors);
        }

        private void HandleParameters(string code, Indicator indicator)
        {
            var parameters = ParametersParser.Parse(code).ToArray();

            indicator.Parameters = parameters;
        }

        private void HandleFields(string code, Indicator indicator)
        {
            var onlyFields = code
                .RemoveMq4Properies()
                .RemoveMq4Parameters()
                .RemoveFunctions()
                .SplitDeclarations()
                .RemoveMq4Buffers(indicator.Buffers)
                .FixArrayDeclarations();

            indicator.Code.FieldsDeclarations = onlyFields;
        }

        private void HandleInit(Indicator indicator, IEnumerable<Function> mq4Functions, ParsingErrors parsingErrors)
        {
            var initFunction = mq4Functions.FirstOrDefault(function => function.Name == "Mq4Init");

            if (initFunction == null)
                return;

            var methodCalls = MethodCallsParser.Parse(initFunction.Body).ToArray();

            HandleBufferIndexes(methodCalls, indicator);
            HandleIndexStyles(methodCalls, indicator, parsingErrors);
        }

        private void HandleIndexStyles(MethodCall[] methodCalls, Indicator indicator, ParsingErrors parsingErrors)
        {
            var setIndexStyleCalls = methodCalls.Where(call => call.MethodName == "SetIndexStyle");
            var indexesStyles = new Dictionary<int, DrawingShapeStyle>();
            foreach (var methodCall in setIndexStyleCalls)
            {
                int index;
                if (!int.TryParse(methodCall.Parameters[0], out index))
                {
                    parsingErrors.Add(ErrorType.NotSupportedWarning, "SetIndexStyle", "Can't cast to int: " + methodCall.Parameters[0]);
                    continue;
                }
                var drawingShapeStyle = methodCall.Parameters[1].ToDrawingShapeStyle();
                indexesStyles[index] = drawingShapeStyle;
            }
            foreach (var keyValuePair in indexesStyles)
            {
                indicator.Styles[keyValuePair.Key] = keyValuePair.Value;
            }
        }

        private void HandleBufferIndexes(IEnumerable<MethodCall> methodCalls, Indicator indicator)
        {
            var setIndexBufferCalls = methodCalls.Where(call => call.MethodName == "SetIndexBuffer");

            var indexesBuffers = new Dictionary<int, string>();
            foreach (var methodCall in setIndexBufferCalls)
            {
                var index = int.Parse(methodCall.Parameters[0]);
                var value = methodCall.Parameters[1];

                indexesBuffers[index] = value;
            }

            indicator.Buffers = indexesBuffers.OrderBy(pair => pair.Key).Select(pair => pair.Value).ToArray();
        }

        private static void HandleProperties(string code, Indicator result)
        {
            var properties = PropertiesParser.Parse(code).ToArray();

            result.IsDrawingOnChartWindow = properties.Any(property => property.Name == "indicator_chart_window");

            var buffersProperty = properties.FirstOrDefault(property => property.Name == "indicator_buffers");
            if (buffersProperty != null)
                result.BuffersCount = int.Parse(buffersProperty.Value);

            var colorProperties = properties.Where(property => property.Name.StartsWith("indicator_color"));
            foreach (var property in colorProperties)
            {
                var index = int.Parse(property.Name["indicator_color".Length].ToString());
                result.Colors[index - 1] = property.Value;
            }
        }
    }
}