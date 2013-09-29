using System.Collections.Generic;

namespace _2calgo.Model
{
    public class Code
    {
        public string FieldsDeclarations { get; set; }

        public List<Function> Functions { get; set; }

        public Code()
        {
            Functions = new List<Function>();
        }
    }
}