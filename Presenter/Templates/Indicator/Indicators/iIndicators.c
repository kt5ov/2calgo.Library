#region MQ4 Indicators

        private DataSeries ToMarketSeries(int constant)
        {
            switch (constant)
            {
                case PRICE_OPEN:
                    return MarketSeries.Open;
                case PRICE_HIGH:
                    return MarketSeries.High;
                case PRICE_LOW:
                    return MarketSeries.Low;
                case PRICE_CLOSE:
                    return MarketSeries.Close;
                case PRICE_MEDIAN:
                    return MarketSeries.Median;       
                case PRICE_TYPICAL:
                    return MarketSeries.Typical;    
                case PRICE_WEIGHTED:
                    return MarketSeries.Weighted;
            }
            throw new NotImplementedException("Converter doesn't support working with this type of AppliedPrice");
        }   

        private static MovingAverageType ToMaType(int constant)
        {
            switch (constant)
            {
                case MODE_SMA:
                    return MovingAverageType.Simple;
                case MODE_EMA:
                    return MovingAverageType.Exponential;
                case MODE_LWMA:
                    return MovingAverageType.Weighted;
                default:
                  throw new ArgumentOutOfRangeException("Not supported moving average type");
            }
        }

#region iMA
        private double iMA(string symbol, int timeframe, int period, int ma_shift, int ma_method, int applied_price, int shift)
        {
            ValidateSymbolAndTimeFrame(symbol, timeframe);            
            if (ma_shift != 0)
                throw new NotImplementedException(NotSupportedMaShift);

            var series = ToMarketSeries(applied_price);
      
            return CalculateiMA(series, period, ma_method, shift);
        }       
                
        private double iMAOnArray(Mq4DataSeries invertedDataSeries, int total, int period, int ma_shift, int ma_method, int shift) 
        {
          return CalculateiMA(invertedDataSeries.OutputDataSeries, period, ma_method, shift);
        }
        
        private double CalculateiMA(DataSeries dataSeries, int period, int ma_method, int shift)
        {     
            if (ma_method == MODE_SMMA)
            {
                return CalculateWellesWilderSmoothing(dataSeries, period, shift);
            }

            var maType = ToMaType(ma_method);            
            var indicator = _cashedStandardIndicators.MovingAverage(dataSeries, period, maType);

            return indicator.Result[_currentIndex - shift];
        }        
        
        private double CalculateWellesWilderSmoothing(DataSeries dataSeries, int period, int shift)
        {
            var indicator = _cashedStandardIndicators.WellesWilderSmoothing(dataSeries, period);
            
            return indicator.Result[_currentIndex - shift];
        }
#endregion //iMA

#region iRSI
        private double iRSI(string symbol, int timeframe, int period, int applied_price, int shift)
        {
            ValidateSymbolAndTimeFrame(symbol, timeframe);            
            
            var series = ToMarketSeries(applied_price);
      
            return CalculateRsi(series, period, shift);
        }       
                
        private double iRSIOnArray(Mq4DataSeries invertedDataSeries, int total, int period, int shift) 
        {
          return CalculateRsi(invertedDataSeries.OutputDataSeries, period, shift);
        }
        
        private double CalculateRsi(DataSeries dataSeries, int period, int shift)
        {     
            var indicator = _cashedStandardIndicators.RelativeStrengthIndex(dataSeries, period);
            return indicator.Result[_currentIndex - shift];
        }

#endregion //iRSI     

#region iBands
        private double iBands(string symbol, int timeframe, int period, int deviation, int bands_shift, int applied_price, int mode, int shift)
        {
            ValidateSymbolAndTimeFrame(symbol, timeframe);                        
            if (bands_shift != 0)
                throw new NotImplementedException(NotSupportedBandsShift);
            
            var series = ToMarketSeries(applied_price);
      
            return CalculateBands(series, period, deviation, mode, shift);
        }       
                
        private double iBandsOnArray(Mq4DataSeries invertedDataSeries, int total, int period, int deviation, int bands_shift, int mode, int shift) 
        {
            if (bands_shift != 0)
                throw new NotImplementedException(NotSupportedBandsShift);
            
            return CalculateBands(invertedDataSeries.OutputDataSeries, period, deviation, mode, shift);
        }
        
        private double CalculateBands(DataSeries dataSeries, int period, int deviation, int mode, int shift)
        {     
            var indicator = _cashedStandardIndicators.BollingerBands(dataSeries, period, deviation, MovingAverageType.Simple);

            switch (mode)
            {
              case MODE_MAIN:
                return indicator.Main[_currentIndex - shift];
              case MODE_UPPER:
                return indicator.Top[_currentIndex - shift];
              case MODE_LOWER:
                return indicator.Bottom[_currentIndex - shift];
            }

            return 0;
        }

#endregion //iBands   

#region iADX
        private Mq4Double iADX(string symbol, int timeframe, int period, int applied_price, int mode, int shift)
        {
            ValidateSymbolAndTimeFrame(symbol, timeframe);            

            if (applied_price != PRICE_CLOSE)            
              throw new NotImplementedException(AdxSupportsOnlyClosePrice);            
      
            return CalculateAdx(period, mode, shift);
        }      
        
        private Mq4Double CalculateAdx(int period, int mode, int shift)
        {     
            var indicator = _cashedStandardIndicators.DirectionalMovementSystem(period);            
            switch (mode)
            {
              case MODE_MAIN:
                return indicator.ADX[_currentIndex - shift];
              case MODE_PLUSDI:
                return indicator.DIPlus[_currentIndex - shift];
              case MODE_MINUSDI:
                return indicator.DIMinus[_currentIndex - shift];
            }
            return 0;
        }

#endregion //iADX    

#region iATR
        private double iATR(string symbol, int timeframe, int period, int shift)
        {
            ValidateSymbolAndTimeFrame(symbol, timeframe);                        
                  
            return CalculateATR(period, shift);
        }       
                
        private double CalculateATR(int period, int shift)
        {     
            var indicator = _cashedStandardIndicators.ATR(period);

            return indicator.Result[_currentIndex - shift];            
        }

#endregion //iATR   

#region iMACD
        private double iMACD(string symbol, int timeframe, int fast_ema_period, int slow_ema_period, int signal_period, int applied_price, int mode, int shift)
        {
            ValidateSymbolAndTimeFrame(symbol, timeframe);                                    
            if (applied_price != PRICE_CLOSE)
            {
                throw new Exception("cTrader doesn't support source parameter for MACD");
            }
      
            return CalculateMACD(fast_ema_period, slow_ema_period, signal_period, mode, shift);
        }       
        
        private double CalculateMACD(int fast_ema_period, int slow_ema_period, int signal_period, int mode, int shift)
        {     
            var indicator = _cashedStandardIndicators.MACD(fast_ema_period, slow_ema_period, signal_period);

            switch (mode)
            {
              case MODE_MAIN:
                return indicator.Histogram[_currentIndex - shift];
              default:
                return indicator.Signal[_currentIndex - shift];
            }
        }

#endregion //iMACD 

#region iCCI
        private double iCCI(string symbol, int timeframe, int period, int applied_price, int shift)
        {
            ValidateSymbolAndTimeFrame(symbol, timeframe);                        
                        
            var series = ToMarketSeries(applied_price);
      
            return CalculateCCI(series, period, shift);
        }       
                
        private double iCCIOnArray(Mq4DataSeries invertedDataSeries, int period, int shift) 
        {            
            return CalculateCCI(invertedDataSeries.OutputDataSeries, period, shift);
        }
        
        private double CalculateCCI(DataSeries dataSeries, int period, int shift)
        {     
            var indicator = _cashedStandardIndicators.CommodityChannelIndex(period);

            return indicator.Result[_currentIndex - shift];
        }

#endregion //iCCI

#region iStdDev
        private double iStdDev( string symbol, int timeframe, int ma_period, int ma_shift, int ma_method, int applied_price, int shift)
        {
            ValidateSymbolAndTimeFrame(symbol, timeframe);            
            if (ma_shift != 0)
                throw new NotImplementedException(NotSupportedMaShift);

            var series = ToMarketSeries(applied_price);
      
            return CalculateiStdDev(series, ma_period, ma_shift, ma_method, shift);
        }       
                
        private double iStdDevOnArray(Mq4DataSeries invertedDataSeries, int total, int ma_period, int ma_shift, int ma_method, int shift) 
        {
          return CalculateiStdDev(invertedDataSeries.OutputDataSeries, ma_period, ma_shift, ma_method, shift);
        }
        
        private double CalculateiStdDev(DataSeries dataSeries, int ma_period, int ma_shift, int ma_method, int shift)
        {     
            if (ma_shift != 0)
                throw new Exception(NotSupportedMaShift);

            var maType = ToMaType(ma_method);            
            var indicator = _cashedStandardIndicators.StandardDeviation(dataSeries, ma_period, maType);

            return indicator.Result[_currentIndex - shift];
        }        
#endregion //iStdDev

#region iWPR
        private double iWPR(string symbol, int timeframe, int period, int shift)
        {
            ValidateSymbolAndTimeFrame(symbol, timeframe);
                   
            var indicator = _cashedStandardIndicators.WilliamsPctR(period);

            return indicator.Result[_currentIndex - shift];
        }        
#endregion //iWPR

#region iSAR
        private double iSAR(string symbol, int timeframe, double step, double maximum, int shift)
        {
            ValidateSymbolAndTimeFrame(symbol, timeframe);
                   
            var indicator = _cashedStandardIndicators.ParabolicSAR(step, maximum);

            return indicator.Result[_currentIndex - shift];
        }        
#endregion //iSAR

#endregion //MQ4 Indicators   