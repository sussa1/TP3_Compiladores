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
            movePoint(newX+2, newX*2);
        }
    };
    movePoint3(newX: Int): Point {
        {
            movePoint2(newX+2);
        }
    };
};