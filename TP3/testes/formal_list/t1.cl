class Point {
    x : Int <- 0;
    y : Int <- 0;
    movePoint (newx : Int, newy : Int) : Point {
        {       
            x <- newx;
            y <- newy;
            self;
        }
    };
};
