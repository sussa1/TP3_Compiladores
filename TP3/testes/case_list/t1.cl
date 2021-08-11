class Point {
    y : Int <- 0;
    
    movePoint(): Point {
        {
            case y of 
                a : Int => 2;
            esac;
            self;
        }
    };

};
