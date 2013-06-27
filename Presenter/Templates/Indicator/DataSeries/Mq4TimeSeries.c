    internal class Mq4TimeSeries
    {
        private readonly TimeSeries _timeSeries;
        private int _currentIndex;
        private static readonly DateTime StartDateTime = new DateTime(1970, 1, 1);

        public Mq4TimeSeries(TimeSeries timeSeries)
        { 
            _timeSeries = timeSeries;
        }

        public void SetCurrentIndex(int index)
        {
            _currentIndex = index;
        }       

        public static int ToInteger(DateTime dateTime)
        {
          return (int)(dateTime - StartDateTime).TotalSeconds;
        }
            
        public static DateTime ToDateTime(int seconds)
        {
          return StartDateTime.AddSeconds(seconds);
        }
                
        public int this[int index]
        {
            get 
            { 
                if (index >= _timeSeries.Count)
                  return 0;
                  
                DateTime dateTime = _timeSeries[_currentIndex - index];

                return ToInteger(dateTime); 
            }
        }
    }