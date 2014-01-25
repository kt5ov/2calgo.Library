using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Threading;
using _2calgo.Model;
using _2calgo.Parser.CodeAdapter;
using _2calgo.Parser.Errors;
using cAlgo.API;
using System;

namespace _2calgo.Parser
{
    public class Mq4Parser
    {
        public ParsingResult Parse(string code, AlgoType algotype)
        {
            Thread.CurrentThread.CurrentCulture = CultureInfo.InvariantCulture;
            var parsingErrors = new ParsingErrors();

            var algo = new Algo
                {
                    Mq4Code = code, 
                    AlgoType = algotype
                };
            string[] customIndicators;

            code = code
                .RemoveComments()
                .HandleParsingErrors(parsingErrors);

            if (parsingErrors.Errors.Any(error => error.ErrorType >= ErrorType.NotSupportedCriticalError))
                return new ParsingResult(algo, parsingErrors.Errors);
            
            code = code
                .ReplaceDateTimeToInt()
                .ReplaceDateConstants()
                .ReplaceDefines()
                .ReplaceCSharpKeyWords()
                .RemoveDotsFromNames()
                .AddZeroToDecimalNumbers()
                .Replace("$", string.Empty)
                .Replace("?", "_")
                .RemoveIncludes()
                .ReplaceMq4RgbColorsToKnownColors()
                .ReplaceColorToInt()
                .ReplaceCAlgoKeyWords()
                .ReplacePrintToMq4Print()
                .RenameFunctions()
                .AddRefModifiers()
                .AddTypeParameterToICustom(out customIndicators);

            HandleProperties(code, algo);
            HandleParameters(code, algo);
            HandleFunctions(code, algo, parsingErrors);
            HandleFields(code, algo);

            algo.Code.ExtractStaticVariablesToFields();
            algo.Code.ReplaceSimpleTypesToMq4Types();
            algo.Code.RenameStandardFunctions();
            algo.Code.AddMq4InitFunctionIfDoesNotExist();
            algo.CustomIndicators = customIndicators;

            return new ParsingResult(algo, parsingErrors.Errors);
        }

        private void HandleFunctions(string code, Algo algo, ParsingErrors parsingErrors)
        {
            var withoutProperties = code.RemoveMq4Properies();
            var mq4Functions = FunctionsParser.Parse(withoutProperties).ToArray();
            algo.Code.Functions = mq4Functions.ToList();
            HandleInit(algo, mq4Functions, parsingErrors);
        }

        private static void HandleParameters(string code, Algo algo)
        {
            var parameters = ParametersParser.Parse(code).ToArray();

            algo.Parameters = parameters;
        }

        private static void HandleFields(string code, Algo algo)
        {
            var onlyFields = code
                .RemoveMq4Properies()
                .RemoveParameters()
                .RemoveFunctions()
                .SplitDeclarations()
                .RemoveMq4Buffers(algo.Buffers)
                .RemoveReturnStatements()
                .ReplaceArraysToMq4Arrays();

            algo.Code.FieldsDeclarations = onlyFields;
        }

        private void HandleInit(Algo algo, IEnumerable<Function> mq4Functions, ParsingErrors parsingErrors)
        {
            var initFunction = mq4Functions.FirstOrDefault(function => function.Name == "init");

            if (initFunction == null)
                return;

            var methodCalls = MethodCallsParser.Parse(initFunction.Body).ToArray();

            HandleBufferIndexes(methodCalls, algo);
            HandleLevels(methodCalls, algo);
            HandleIndexStyles(methodCalls, algo, parsingErrors);
        }

        private static void HandleIndexStyles(MethodCall[] methodCalls, Algo algo, ParsingErrors parsingErrors)
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

                if (methodCall.Parameters.Length >= 4)
                {
                    int width;
                    if (int.TryParse(methodCall.Parameters[3], out width))
                        algo.Widths[index] = width;
                }
            }
            foreach (var keyValuePair in indexesStyles)
            {
                algo.Styles[keyValuePair.Key] = keyValuePair.Value;
            }
        }

        private void HandleBufferIndexes(IEnumerable<MethodCall> methodCalls, Algo algo)
        {
            var setIndexBufferCalls = methodCalls.Where(call => call.MethodName == "SetIndexBuffer");

            var indexesBuffers = new Dictionary<int, string>();
            foreach (var methodCall in setIndexBufferCalls)
            {
                var index = int.Parse(methodCall.Parameters[0]);
                var value = methodCall.Parameters[1];

                indexesBuffers[index] = value;
            }

            algo.Buffers = indexesBuffers.OrderBy(pair => pair.Key).Select(pair => pair.Value).ToArray();
        }

        private void HandleLevels(IEnumerable<MethodCall> methodCalls, Algo algo)
        {
            var setLevelCalls = methodCalls.Where(call => call.MethodName == "SetLevelValue");

            var levels = new Dictionary<int, double>();
            foreach (var methodCall in setLevelCalls)
            {
                var levelIndex = int.Parse(methodCall.Parameters[0]);
                double value;
                if (double.TryParse(methodCall.Parameters[1], out value))
                    levels[levelIndex] = value;
            }

            var levelValues = levels.Select(pair => pair.Value);
            algo.Levels.AddRange(levelValues);
        }

        private static void HandleProperties(string code, Algo result)
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
                result.Colors[index - 1] = GetKnownColor(property.Value);
            }

            var widthProperties = properties.Where(property => property.Name.StartsWith("indicator_width"));
            foreach (var property in widthProperties)
            {
                int index;
                int width;
                if (int.TryParse(property.Name["indicator_width".Length].ToString(), out index) 
                    && int.TryParse(property.Value, out  width))
                    result.Widths[index - 1] = width;
            }

            var levelProperties = properties.Where(property => property.Name.StartsWith("indicator_level"));
            foreach (var property in levelProperties)
            {
                double levelValue;
                if (double.TryParse(property.Value, out  levelValue))
                    result.Levels.Add(levelValue);
            }
        }
        
        private static string GetKnownColor(string mq4Color)
        {
            Colors color;
            if (!Enum.TryParse(mq4Color, out color))
                return "Green";

            return color.ToString();
        }
    }
}