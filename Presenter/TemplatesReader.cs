using System;
using System.IO;
using System.Windows;

namespace _2calgo.Presenter
{
    public class TemplatesReader
    {
        private const string FolderPath = @"pack://application:,,,/2calgo.Presenter;component/Templates/";

        public static string Read(string path)
        {
            lock (SyncObject)
            {
                if (!UriParser.IsKnownScheme("pack"))
                    new Application();

                var resourceInfo = Application.GetResourceStream(new Uri(FolderPath + path, UriKind.Absolute));
                using (var reader = new StreamReader(resourceInfo.Stream))
                {
                    return reader.ReadToEnd();
                }
            }
        }

        private static readonly object SyncObject = new object();
    }
}