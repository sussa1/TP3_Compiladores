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
   head(): Int { 
      head.getVal()
   };
   tail()  : List { 
      tail.getVal()
   };
};

class Stack inherits List {
   top()- Int {
      tail()
   }
};