using System;
using System.Collections.Generic;
using System.Linq;

namespace _2calgo.Presenter
{
    static class IndicatorTemplateProvider
    {
        private static readonly IEnumerable<string> PartNames = new string[]
            {
                "Custom indicators",
                "Debug",
                "Predefined variables",
                "DataSeries",
                "Constants",
                "Math and Trig",
                "Conversion functions",
                "Window functions",
                "String functions",
                "Date and Time functions",
                "Messages",
                "Validation",
                "Timeseries access",
                "Indicators",
                "Common functions",
                "Array functions",
            };

        public static string GetTemplate()
        {
            var mainTemplate = TemplatesReader.Read("Indicator/MainTemplate.c");
            var parts = PartNames.Select(part => TemplatesReader.Read("Indicator/" + part + ".c"));
            var allParts = string.Join(Environment.NewLine, parts);

            return mainTemplate.Replace("#Conditional_Part_PLACE_HOLDER#", allParts);
        }
    }
}
