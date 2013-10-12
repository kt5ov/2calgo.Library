using System;
using System.Collections.Generic;
using System.Linq;

namespace _2calgo.Presenter.PartsProviders
{
    public static class IndicatorInnerPartsProvider
    {
        private const string FolderPath = @"pack://application:,,,/2calgo.Presenter;component/Templates/Indicator/InnerParts/";

        private static readonly IEnumerable<string> InnerParts = new string[0]
            {
            };

        public static string Get()
        {
            var partsContents = InnerParts.Select(part => StringResourceReader.Read(FolderPath + part));
            var allParts = string.Join(Environment.NewLine, partsContents);
            return allParts;
        }
    }
}