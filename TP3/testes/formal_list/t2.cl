class Point {
    x : Int <- 0;
    y : Int <- 0;
    movePoint (newx : Int) : Point {
        {       
            x <- newx;
            y <- 0;
            self;
        }
    };
};
