class Node {
   val = Int;
   prox: Node;
   getVal() : Int  {
      val
   };
   getProx() : Node  {
      prox
   };
};

class List {
   head: Node;
   tail: Node;
   isNil(): Bool {
      head = error; 
      head = tail;
   };
   gethead(): Int { 
      head.getVal()
   };
   gettail()  : List { 
      tail.getVal()
   };
};

class Stack inherits List {
   top()- Int {
      gettail()
   };
};