using System;
using System.Linq;
using System.Text.RegularExpressions;
using _2calgo.Parser.Extensions;

namespace _2calgo.Parser.CodeAdapter
{
    public static class Mq4Dates
    {
        private static readonly DateTime StartDateTime = new DateTime(1970, 1, 1);

        private static readonly Regex Regex =
            new Regex(
                @"D\s*'((?<datePart>(?<year>\d+)\.(?<month>\d+)\.(?<day>\d+)){0,1}(?<timePart>\s*(?<hour>\d+)(\:(?<min>\d+)(\:(?<sec>\d+)){0,1}){0,1}){0,1}){0,1}'", RegexOptions.Compiled);

        public static string ReplaceDateConstants(this string code)
        {
            while (true)
            {
                var match = Regex.Match(code);
                if (!match.Success)
                    return code;

                DateTime dateTime;
                if (match.Groups["datePart"].Value == string.Empty)
                    dateTime = DateTime.UtcNow.Date;
                else
                {
                    var year = Math.Max(match.GetIntValue("year").Value, match.GetIntValue("day").Value);
                    var month = match.GetIntValue("month").Value;
                    var day = Math.Min(match.GetIntValue("year").Value, match.GetIntValue("day").Value);
                    dateTime = new DateTime(year, month, day);
                }

                if (match.Groups["timePart"].Value != string.Empty)
                {
                    var hour = match.GetIntValue("hour");
                    var min = match.GetIntValue("min");
                    var sec = match.GetIntValue("sec");

                    dateTime = dateTime.AddHours(hour.Value);
                    if (min != null)
                        dateTime = dateTime.AddMinutes(min.Value);
                    if (sec != null)
                        dateTime = dateTime.AddSeconds((sec.Value));
                }

                var replacement = ToInteger(dateTime);
                code = code.Replace(match.Value, replacement.ToString());
            }
        }

        private static int ToInteger(DateTime dateTime)
        {
            return (int)(dateTime - StartDateTime).TotalSeconds;
        }
            
    }
}