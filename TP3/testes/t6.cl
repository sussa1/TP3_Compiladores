class Point {
    x : Int <- 0;
    y : Int <- 0;
    movePoint (newx : Int, newy : Int) : Point {
        let wx:Int <- 0 in 
        {
            x <- newx;
            y <- newy;
            self;
        }
    };
};