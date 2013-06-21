using System;
using System.CodeDom.Compiler;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Microsoft.CSharp;
using cAlgo.API;

namespace _2calgo.Library
{
    public class CSharpCompiler
    {
        private const int ShowWarnings = 2;
        
        public CompilerError[] Compile(string sourceCode, string outputAssemblyPath)
        {
            var compilerParameters = new CompilerParameters
            {
                GenerateInMemory = false,
                OutputAssembly = outputAssemblyPath,
                WarningLevel = ShowWarnings
            };

            foreach (var referenceLocation in GetStandardReferences())
            {
                compilerParameters.ReferencedAssemblies.Add(referenceLocation);
            }

            var providerOptions = new Dictionary<string, string> { { "CompilerVersion", "v4.0" } };
            var provider = new CSharpCodeProvider(providerOptions);
            var compilerResults = provider.CompileAssemblyFromSource(compilerParameters, new[] { sourceCode });
            var errorsAndWarnings = compilerResults.Errors.Cast<CompilerError>().ToArray();
            return errorsAndWarnings;
        }

        private static readonly IList<string> StandardAssemblyList = new List<string>
            {
                "mscorlib",
                "System.Core"
            };

        private List<string> _filePaths;

        private IEnumerable<string> GetStandardReferences()
        {
            if (_filePaths == null)
            {
                _filePaths = AppDomain.CurrentDomain.GetAssemblies()
                    .Where(assembly => !assembly.IsDynamic
                        && File.Exists(assembly.Location)
                            && StandardAssemblyList.Contains(assembly.GetName().Name))
                    .Select(assembly => assembly.Location)
                    .ToList();

                var cAlgoApiAssemblyLocation = typeof (Robot).Assembly.Location;
                _filePaths.Add(cAlgoApiAssemblyLocation);
            }
            return _filePaths;
        }
    }
}