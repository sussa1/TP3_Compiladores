class Pai {

};

class Filha inherits Pai {
    
};

class Main inherits IO {
    attr : Filha <- new Filha;

    main() : Object {
        {
            out_int(1+2);
            out_string("\n");
            out_int(2*3);
            out_string("\n");
            out_int(4/2);
            out_string("\n");
            out_int(2-1);
            out_string("\n");
            case attr of 
                pai : Pai => out_string("Certo\n");
            esac;
            case attr of 
                pai : Pai => out_string("Errado\n");
                filha : Filha => out_string("Certo\n");
            esac;
        }
    };
};