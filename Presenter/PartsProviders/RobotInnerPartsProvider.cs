using System;
using System.Collections.Generic;
using System.Linq;

namespace _2calgo.Presenter.PartsProviders
{
    public class RobotInnerPartsProvider
    {
        private const string FolderPath = @"pack://application:,,,/2calgo.Presenter;component/Templates/Robot/InnerParts/";
        private const string Account = "Account";

        private static readonly IEnumerable<string> InnerParts = new[]
            {
                "Trading functions.c",
                "Execution errors.c",
                Account + "/Orders.c",
                Account + "/Account information.c",
            };

        public static string Get()
        {
            var partsContents = InnerParts.Select(part => StringResourceReader.Read(FolderPath + part));
            var allParts = string.Join(Environment.NewLine, partsContents);
            return allParts;
        } 
    }
}