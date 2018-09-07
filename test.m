		
    y =70;
    u =168;
    v =114;
    rgb1 = (y + 1.402*(u - 128));
		rgb2 = (y - 0.344*(u - 128) - 0.714*(v - 128));
		rgb3 = (y + 1.772*(v - 128));

    
    rgb4 = (y + (u - 128) + ((104*(u - 128))/256));
		rgb5 = (y - (89*(v - 128)/ 256) - ((183*(u - 128))/256));
		rgb6 = (y + (v - 128) + ((199*(v - 128))/256));
        
        
    x1 = dec2hex(int8(rgb1));
    x2 = dec2hex(int8(rgb2));
    x3 = dec2hex(int8(rgb3));    
    x4 = dec2hex(int8(rgb4));
    x5 = dec2hex(int8(rgb5));
    x6 = dec2hex(int8(rgb6));