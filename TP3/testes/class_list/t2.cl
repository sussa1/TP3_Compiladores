class Node {
   val = Int;
   prox: Node;
}

class List {
   head: Node;
   tail: Node;
   isNil(): Bool {
      head = error; 
      head = tail;
   };
   head(): Int { 
      head.val
   };
   tail()  : List { 
      tail.val
   };
};

class Stack inherits List {
   top()- Int {
      tail()
   }
};