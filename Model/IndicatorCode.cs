using System.Collections.Generic;

namespace _2calgo.Model
{
    public class IndicatorCode
    {
        public string FieldsDeclarations { get; set; }

        public IEnumerable<Function> Functions { get; set; }

        public IndicatorCode()
        {
            Functions = new Function[0];
        }
    }
}