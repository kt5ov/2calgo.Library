#References_PLACE_HOLDER#// ------------------------------------------------------------                   
// Paste this code into your cAlgo editor. 
// -----------------------------------------------------------
using System;
using System.Collections.Generic;
using cAlgo.API;
using cAlgo.API.Indicators;
using cAlgo.API.Internals;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Collections;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO;
using System.Reflection;
using System.Threading;
using System.Diagnostics;
using System.Runtime.InteropServices;
using Microsoft.Win32;
// ---------------------------------------------------------------------------                   
// Converted from MQ4 to cAlgo with http://2calgo.com
// ---------------------------------------------------------------------------

namespace cAlgo.Indicators
{
    [Indicator(ScalePrecision = 5, AutoRescale = false, IsOverlay = #IsDrawingOnChartWindow_PLACE_HOLDER#)]
	[Levels(#Levels_PLACE_HOLDER#)]
	public class #AlgoName_PLACE_HOLDER# : Indicator
    {
#Mq4Functions_PLACE_HOLDER#

[Conditional("SendMail")]
//{
		[Parameter("Email from", DefaultValue = "your_email@gmail.com")]
        public string EmailAddressFrom { get; set; }

        [Parameter("Email to", DefaultValue = "your_email@gmail.com")]
        public string EmailAddressTo { get; set; }
//}

#Parameters_PLACE_HOLDER#

#Lines_declarations_PLACE_HOLDER#
     
#Mq4Fields_PLACE_HOLDER#
		
#ColorParameters_PLACE_HOLDER#
				
#WidthParameters_PLACE_HOLDER#

#LevelParameters_PLACE_HOLDER#
        
        protected override void Initialize()
        {
#Initialize_buffers_PLACE_HOLDER#
#Initialize_AllOutputDataSeries_PLACE_HOLDER#

			CommonInitialize();			            
#DebugActions_PLACE_HOLDER#            
        }
        
#Inverted_buffers_declarations_PLACE_HOLDER#        

        List<Mq4OutputDataSeries> AllBuffers = new List<Mq4OutputDataSeries>();		
		public List<DataSeries> AllOutputDataSeries = new List<DataSeries>();

	private bool _initialized;
    public override void Calculate(int index)
    {
		try
		{
			_currentIndex = index;
			#Buffers_SetCurrentIndex_PLACE_HOLDER#

			if (IsLastBar) 
			{   
				if (!_initialized)
				{
					Mq4Init();
					_initialized = true;
				}
        
				Mq4Start();       
				_indicatorCounted = index;
			}
		}
		catch(Exception e)
		{
			#HandleException_PLACE_HOLDER#
			throw;
		}
    }	

#InnerParts_PLACE_HOLDER#
	}

	//Custom Indicators Place Holder

#OuterParts_PLACE_HOLDER#

	static class Mq4LineStyles
    {
      public static LineStyle ToLineStyle(int style)
      {
        switch (style)
        {
          case 1: 
            return LineStyle.Lines;
          case 2: 
            return LineStyle.Dots;
          case 3: 
          case 4: 
            return LineStyle.LinesDots;
          default:
            return LineStyle.Solid;
        }
      }
    }
}