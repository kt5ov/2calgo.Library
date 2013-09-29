	[Conditional("OBJ_LABEL")] 
	class Mq4Label : Mq4Object
    {
        public Mq4Label(string name, int type, ChartObjects chartObjects) 
            : base(name, type, chartObjects)
        {
        }

        public string Text { get; set; }

        private int X 
        {
            get
            {
            return Get(OBJPROP_XDISTANCE);
            }
        }

        private int Corner
        {
            get
            {
            return Get(OBJPROP_CORNER);
            }
        }

        private int Y
        {
            get
            {
            return Get(OBJPROP_YDISTANCE);
            }
        }

        private string MultiplyString(string str, int count)
        {
			var stringBuilder = new StringBuilder();
			for (var i = 0; i < count; i++)         
				stringBuilder.Append(str);
			return stringBuilder.ToString();            
        }    

		private string GetSpaces(int count)
        {
			return MultiplyString(" ", count);
        }
          
		private string GetNewLines(int count)
        {
			return MultiplyString("\r\n", count);
        }

        public override void Draw()
        {                               
            const double spaceWidth = 5.4;
			const double lineHeight = 20;

			var xSpaces = GetSpaces((int)Math.Ceiling(X / spaceWidth));
			var ySpaces = GetNewLines((int)Math.Ceiling(Y / lineHeight));
        
			switch(Corner)
			{
				case 0:
					DrawText(Name, ySpaces + xSpaces + Text, StaticPosition.TopLeft, Color);
				break;
				case 1:
					DrawText(Name, ySpaces + Text + xSpaces + '.', StaticPosition.TopRight, Color);
				break;          
				case 2:
					DrawText(Name, xSpaces + Text + ySpaces, StaticPosition.BottomLeft, Color);
				break;          
				case 3:
					DrawText(Name, Text + xSpaces + '.' + ySpaces, StaticPosition.BottomRight, Color);
				break;  
			}
        }
    }