class Main inherits IO {
    irmao : Main;
    main() : SELF_TYPE {
        case irmao of 
            irmao1 : Main => irmao1;
            irmao2 : Int => irmao2;
            irmao3 : Bool => irmao3;
        esac;
    };
};