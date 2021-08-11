class A {
};

Class b inherits A {
};

Class C inherits a {
};

Class D inherts A {
};

Class E inherits A {
    x : Int <- 0;
    y : Int - 0;
    movePoint (newx : Int, newy : Int) : Point {
        {       
            x <- newx;
            y - newy;
            self;
        }
    };
    movePoint2 (newx - Int, newy Int) : Point {
        {
            x <- newz;
            movePoint(newX?2, newX^2);
            case y of 
                a : Int => 2 <> 1;
                b : Bool => Bool;
            esac;
        }
    };
    movePoint3(): Point {
        {
            let wx-Int, wz:Int, wy?Int <- 0, w:Int in 
            {
                x <- newx;
                y <- newy;
                self;
            };
            self;
        }
    };
};

Class F inherits A {
;

