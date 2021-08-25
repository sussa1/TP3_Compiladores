class Pai {

};

class Filha inherits Pai {
    
};

class Main inherits IO {
    Filha attr <- new Filha;

    main() : Object {
        {
            out_int(1+2);
            out_int(2*3);
            out_int(4/2);
            out_int(2-1);
            case attr of 
                pai : Pai => out_string("Certo");
            esac;
        }
    };
};