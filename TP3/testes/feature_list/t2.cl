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
    movePoint2(newX: Int): Point {
        {
            x <- newX;
            y <- 0;
            self;
        }
    };
    movePoint3(): Point {
        {
            x <- 0
            y <- 0;
            self
        }
    };
};
