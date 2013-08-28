
using System.Collections.Generic;
using System.Text;

namespace _2calgo.Presenter
{
    public class Words
    {
        private readonly HashSet<string> _words = new HashSet<string>();

        public Words(string text)
        {
            var currentWord = new StringBuilder();
            for (var i = 0; i < text.Length; i++)
            {
                if (char.IsLetterOrDigit(text[i]) || text[i] == '_')
                    currentWord.Append(text[i]);
                else if (currentWord.Length > 0)
                {
                    _words.Add(currentWord.ToString());
                    currentWord.Clear();
                }
            }
            if (currentWord.Length > 0)
                _words.Add(currentWord.ToString());
        }

        public bool Contains(string word)
        {
            return true;
//            return _words.Contains(word);
        }
    }
}
