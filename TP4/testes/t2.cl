class ClasseMae {
    valorInteiro(foo : Int, dummy : Int) : Int {
        foo*20
    };
};

class TesteExpression inherits ClasseMae {
    irmao : TesteExpression;
    calc : Int;
    msgString: String <- "O valor eh: ";
    valorInteiro(foo : Int, dummy : Int) : Int {
        foo*10
    };
    testeTodasExpr(foo: Int, dummy: Int) : Bool {
        {       
            let lixo: Int, lixoTeste: TesteExpression, lixoBool: Bool, calc: Int in {
                foo <- self.valorInteiro(foo, dummy);
                dummy <- irmao @ ClasseMae . valorInteiro(foo, dummy);
                foo <- irmao.valorInteiro(foo, dummy);
                lixo <- valorInteiro(foo, dummy);
                lixo <- if valorInteiro(foo, dummy) = 10 then 1 else 2 fi;
                let cont: Int <- 10 in {
                    while 0 <= cont LOOP cont <- cont-1 POOL;
                    calc <- cont;
                };
                lixoTeste <- new TesteExpression;
                lixoBool <- isvoid lixo;
                lixo <- lixo + foo;
                lixo <- lixo - foo;
                lixo <- lixo * foo;
                lixo <- lixo / foo;
                lixo <- ~lixo;
                lixoBool <- lixo < foo*2;
                lixoBool <- lixo-1 <= foo;
                lixoBool <- lixo = foo;
                lixoBool <- NOT lixo = foo;
                lixo <- (lixo + foo) * (lixo + foo);
                lixo <- 10;
                lixo;
                case calc OF
                    calcInt : Int => calcInt-10;
                    calcString : String => calcString.concat("Result");
                    calcBool : Bool => NOT calcBool;
                esac;
            };
        }
    };
};

class Main inherits IO {
    main() : SELF_TYPE {
        let teste : TesteExpression in {
            teste <- new TesteExpression;
            out_string(teste.testeTodasExpr(1, 100));
        }
    };
}