#include <assert.h>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <map>
#include "emit.h"
#include "cool-tree.h"
#include "symtab.h"

enum Basicness     {Basic, NotBasic};
#define TRUE 1
#define FALSE 0

class CgenClassTable;
typedef CgenClassTable *CgenClassTableP;

class CgenNode;
typedef CgenNode *CgenNodeP;

class CgenClassTable : public SymbolTable<Symbol,CgenNode> {
private:
   List<CgenNode> *nds;
   ostream& str;
   int stringclasstag;
   int intclasstag;
   int boolclasstag;

   void generateClassTags();
// The following methods emit code for
// constants and global declarations.

   void code_global_data();
   void code_global_text();
   void code_bools(int);
   void code_select_gc();
   void code_constants();
   void code_prototypeObjects();
   std::vector<CgenNode*> getClassNodes();
   std::vector<std::pair<CgenNode*, std::pair<int, int> > > getClassNodeTagAndSize();
   void code_classNameTable();
   void code_dispatchTables();
   void code_objectInitializer();
   void code_classMethods();
   void code_classPrototypeTable();

// The following creates an inheritance graph from
// a list of classes.  The graph is implemented as
// a tree of `CgenNode', and class names are placed
// in the base class symbol table.

   void install_basic_classes();
   void install_class(CgenNodeP nd);
   void install_classes(Classes cs);
   void build_inheritance_tree();
   void set_relations(CgenNodeP nd);
public:
   CgenClassTable(Classes, ostream& str);
   void code();
   CgenNodeP root();
};

class Scope {
public:
   Scope() : classNode(nullptr) {}

   void newScope() { // Cria novo escopo
      scopeSize.push_back(0);
   }

   void exitScope() { // Sai do escopo atual
      for(int it = 0; it < scopeSize.back(); it++) {
         variableTable.pop_back();
      }
      scopeSize.pop_back();
   }

   int lookUpAttribute(Symbol symbol); // Busca um símbolo que é um atributo

   // Busca um símbolo que é uma variável
   // As variáveis são salvas em ordem reversa
   int lookUpVariable(Symbol symbol) {
      for(int it = variableTable.size() - 1; it >= 0; it--) {
         if(variableTable[it] == symbol) {
            return variableTable.size() - 1 - it;
         }
      }
      return -1;
   }

   // Adiciona uma nova variável
   int addVariable(Symbol symbol) {
      variableTable.push_back(symbol);
      scopeSize[scopeSize.size() - 1]++;
      return variableTable.size() - 1;
   }

   // Adiciona um elemento que foi inserido na pilha mas não é variável
   int addDullElement();   

   // Busca um símbolo que é um parâmetro
   int lookUpParameter(Symbol symbol) {
      for(int it = 0; it < parameterTable.size(); it++) {
         if(parameterTable[it] == symbol) {
            return parameterTable.size() - 1 - it;
         }
      }
      return -1;
   }

   // Adiciona um novo parâmetro
   int addParameter(Symbol symbol) {
      parameterTable.push_back(symbol);
      return parameterTable.size() - 1;
   }

   std::vector<int> scopeSize;
   std::vector<Symbol> variableTable;
   std::vector<Symbol> parameterTable;
   CgenNode* classNode;

};


class CgenNode : public class__class {
private: 
   CgenNodeP parentnd;                        // Parent of class
   List<CgenNode> *children;                  // Children of class
   Basicness basic_status;                    // `Basic' if class is basic
                                              // `NotBasic' otherwise

public:
   CgenNode(Class_ c,
            Basicness bstatus,
            CgenClassTableP class_table);

   void add_child(CgenNodeP child);
   List<CgenNode> *get_children() { return children; }
   void set_parentnd(CgenNodeP p);
   CgenNodeP get_parentnd() { return parentnd; }
   int basic() { return (basic_status == Basic); }
   
   // Métodos auxiliaries para a emissão de código
   // de constantes e declarações globais
   void code_prototypeObject(ostream& str, int tag, int size);
   int getSize();
   void code_attributesPrototypeObject(ostream& str, int& offset);
   void code_dispatchTable(ostream& str);
   void code_objectInitializer(ostream& str);
   void code_attributeInitializer(ostream& str);
   void code_classMethods(ostream& str);
   std::map<Symbol, std::pair<method_class*, Symbol> > getFunctionsOfClass();
};

class BoolConst 
{
 private: 
  int val;
 public:
  BoolConst(int);
  void code_def(ostream&, int boolclasstag);
  void code_ref(ostream&) const;
};

