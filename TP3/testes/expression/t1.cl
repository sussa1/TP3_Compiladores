class ClasseMae {
    valorInteiro(foo : Int, dummy : Int) : Int {
        20
    };
};

class TesteExpression inherits ClasseMae {
    irmao : TesteExpression;
    calc : Int;
    msgString: String <- "O valor eh: ";
    valorInteiro(foo : Int, dummy : Int) : Int {
        10
    };
    testeTodasExpr(foo : Int, dummy : Int) : Bool {
        {       
            variavel1 <- self.valorInteiro(foo, dummy);
            variavel2 <- irmao.valorInteiro(foo, dummy);
            variavel3 <- irmao @ ClasseMae . valorInteiro(foo, dummy);
            variavel4 <- valorInteiro(foo, dummy);
            variavel5 <- if valorInteiro(foo, dummy) = 10 then 1 else 2;
            let cont: Int <- 10 in {
             while 0 <= cont LOOP cont <- cont-1 POOL;
             calc <- cont;
            }
            case calc OF
                calcInt : Int => calcInt-10;
                calcString : String => msgString + calcInt;
                calcBool : Bool => NOT calcBool;
            esac;
            variavel6 <- new TesteExpression;
            variavel7 <- isvoid variavel3;
            variavel8 <- variavel1 + variavel2;
            variavel9 <- variavel1 - variavel2;
            variavel11 <- variavel1 * variavel2;
            variavel12 <- variavel1 / variavel2;
            variavel13 <- ~variavel1;
            variavel14 <- variavel1 < variavel2*2;
            variavel15 <- variavel1-1 <= variavel2;
            variavel16 <- variavel1 = variavel2;
            variavel17 <- NOT variavel1 = variavel2;
            variavel18 <- (variavel1 + variavel2) * (variavel1 + variavel2);
            variavel18
            10;
            "Teste";
            true;
        }
    };
};
