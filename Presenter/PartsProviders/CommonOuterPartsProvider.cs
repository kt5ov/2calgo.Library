using System;
using System.Collections.Generic;
using System.Linq;

namespace _2calgo.Presenter.PartsProviders
{
    public static class CommonOuterPartsProvider
    {
        private const string FolderPath = @"pack://application:,,,/2calgo.Presenter;component/Templates/Common/OuterParts/";

        private static readonly IEnumerable<string> OuterParts = new[]
            {
                "Mq4Double.cs",
                "Mq4DoubleComparer.cs",
                "Mq4String.c",
                "Mq4Char.c",
                "Mq4Null.cs",
                "Comparers.cs",
                "DataSeriesExtensions.c",
                "TimeSeriesExtensions.c",
                "Mq4Colors.c",
                "EventExtensions.c",
                "Mq4LineStyles.c",
                "Mq4TimeSeries.c",
                "ConvertExtensions.c",
            };

        public static string Get()
        {
            var partsContents = OuterParts.Select(part => StringResourceReader.Read(FolderPath + part));
            var allParts = string.Join(Environment.NewLine, partsContents);
            return allParts;
        }
    }
}