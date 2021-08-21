
//**************************************************************
//
// Code generator SKELETON
//
// Read the comments carefully. Make sure to
//    initialize the base class tags in
//       `CgenClassTable::CgenClassTable'
//
//    Add the label for the dispatch tables to
//       `IntEntry::code_def'
//       `StringEntry::code_def'
//       `BoolConst::code_def'
//
//    Add code to emit everyting else that is needed
//       in `CgenClassTable::code'
//
//
// The files as provided will produce code to begin the code
// segments, declare globals, and emit constants.  You must
// fill in the rest.
//
//**************************************************************

#include "cgen.h"
#include "cgen_gc.h"
#include <algorithm>
#include <map>
#include <vector>

extern void emit_string_constant(ostream& str, char *s);
extern int cgen_debug;

std::map<Symbol, std::map<Symbol, int > > offsetClassAttr;
std::map<Symbol, std::map<Symbol, std::pair<method_class*, std::pair<int, Symbol> > > > methodOffsetClassMethod;
std::map<Symbol, CgenNode*> classNodeMap;
std::map<int, CgenNode*> classesByTag;

std::map<Symbol, std::vector<int> > symbolTable;
std::map<int, Symbol> reverseSymbolTable;
std::vector<int> scopes = {0};
int elementsInStack = 0;
CgenNode* currentClass;

//
// Three symbols from the semantic analyzer (semant.cc) are used.
// If e : No_type, then no code is generated for e.
// Special code is generated for new SELF_TYPE.
// The name "self" also generates code different from other references.
//
//////////////////////////////////////////////////////////////////////
//
// Symbols
//
// For convenience, a large number of symbols are predefined here.
// These symbols include the primitive type and method names, as well
// as fixed names used by the runtime system.
//
//////////////////////////////////////////////////////////////////////
Symbol 
       arg,
       arg2,
       Bool,
       concat,
       cool_abort,
       copy,
       Int,
       in_int,
       in_string,
       IO,
       length,
       Main,
       main_meth,
       No_class,
       No_type,
       Object,
       out_int,
       out_string,
       prim_slot,
       self,
       SELF_TYPE,
       Str,
       str_field,
       substr,
       type_name,
       val;
//
// Initializing the predefined symbols.
//
static void initialize_constants(void)
{
  arg         = idtable.add_string("arg");
  arg2        = idtable.add_string("arg2");
  Bool        = idtable.add_string("Bool");
  concat      = idtable.add_string("concat");
  cool_abort  = idtable.add_string("abort");
  copy        = idtable.add_string("copy");
  Int         = idtable.add_string("Int");
  in_int      = idtable.add_string("in_int");
  in_string   = idtable.add_string("in_string");
  IO          = idtable.add_string("IO");
  length      = idtable.add_string("length");
  Main        = idtable.add_string("Main");
  main_meth   = idtable.add_string("main");
//   _no_class is a symbol that can't be the name of any 
//   user-defined class.
  No_class    = idtable.add_string("_no_class");
  No_type     = idtable.add_string("_no_type");
  Object      = idtable.add_string("Object");
  out_int     = idtable.add_string("out_int");
  out_string  = idtable.add_string("out_string");
  prim_slot   = idtable.add_string("_prim_slot");
  self        = idtable.add_string("self");
  SELF_TYPE   = idtable.add_string("SELF_TYPE");
  Str         = idtable.add_string("String");
  str_field   = idtable.add_string("_str_field");
  substr      = idtable.add_string("substr");
  type_name   = idtable.add_string("type_name");
  val         = idtable.add_string("_val");
}

static char *gc_init_names[] =
  { "_NoGC_Init", "_GenGC_Init", "_ScnGC_Init" };
static char *gc_collect_names[] =
  { "_NoGC_Collect", "_GenGC_Collect", "_ScnGC_Collect" };


//  BoolConst is a class that implements code generation for operations
//  on the two booleans, which are given global names here.
BoolConst falsebool(FALSE);
BoolConst truebool(TRUE);

//*********************************************************
//
// Define method for code generation
//
// This is the method called by the compiler driver
// `cgtest.cc'. cgen takes an `ostream' to which the assembly will be
// emmitted, and it passes this and the class list of the
// code generator tree to the constructor for `CgenClassTable'.
// That constructor performs all of the work of the code
// generator.
//
//*********************************************************

void program_class::cgen(ostream &os) 
{
  // spim wants comments to start with '#'
  os << "# start of generated code\n";

  initialize_constants();
  CgenClassTable *codegen_classtable = new CgenClassTable(classes,os);

  os << "\n# end of generated code\n";
}


//////////////////////////////////////////////////////////////////////////////
//
//  emit_* procedures
//
//  emit_X  writes code for operation "X" to the output stream.
//  There is an emit_X for each opcode X, as well as emit_ functions
//  for generating names according to the naming conventions (see emit.h)
//  and calls to support functions defined in the trap handler.
//
//  Register names and addresses are passed as strings.  See `emit.h'
//  for symbolic names you can use to refer to the strings.
//
//////////////////////////////////////////////////////////////////////////////

static void emit_load(char *dest_reg, int offset, char *source_reg, ostream& s)
{
  s << LW << dest_reg << " " << offset * WORD_SIZE << "(" << source_reg << ")" 
    << endl;
}

static void emit_store(char *source_reg, int offset, char *dest_reg, ostream& s)
{
  s << SW << source_reg << " " << offset * WORD_SIZE << "(" << dest_reg << ")"
      << endl;
}

static void emit_load_imm(char *dest_reg, int val, ostream& s)
{ s << LI << dest_reg << " " << val << endl; }

static void emit_load_address(char *dest_reg, const char *address, ostream& s)
{ s << LA << dest_reg << " " << address << endl; }

static void emit_partial_load_address(char *dest_reg, ostream& s)
{ s << LA << dest_reg << " "; }

static void emit_load_bool(char *dest, const BoolConst& b, ostream& s)
{
  emit_partial_load_address(dest,s);
  b.code_ref(s);
  s << endl;
}

static void emit_load_string(char *dest, StringEntry *str, ostream& s)
{
  emit_partial_load_address(dest,s);
  str->code_ref(s);
  s << endl;
}

static void emit_load_int(char *dest, IntEntry *i, ostream& s)
{
  emit_partial_load_address(dest,s);
  i->code_ref(s);
  s << endl;
}

static void emit_move(char *dest_reg, char *source_reg, ostream& s)
{ s << MOVE << dest_reg << " " << source_reg << endl; }

static void emit_neg(char *dest, char *src1, ostream& s)
{ s << NEG << dest << " " << src1 << endl; }

static void emit_add(char *dest, char *src1, char *src2, ostream& s)
{ s << ADD << dest << " " << src1 << " " << src2 << endl; }

static void emit_addu(char *dest, char *src1, char *src2, ostream& s)
{ s << ADDU << dest << " " << src1 << " " << src2 << endl; }

static void emit_addiu(char *dest, char *src1, int imm, ostream& s)
{ s << ADDIU << dest << " " << src1 << " " << imm << endl; }

static void emit_div(char *dest, char *src1, char *src2, ostream& s)
{ s << DIV << dest << " " << src1 << " " << src2 << endl; }

static void emit_mul(char *dest, char *src1, char *src2, ostream& s)
{ s << MUL << dest << " " << src1 << " " << src2 << endl; }

static void emit_sub(char *dest, char *src1, char *src2, ostream& s)
{ s << SUB << dest << " " << src1 << " " << src2 << endl; }

static void emit_sll(char *dest, char *src1, int num, ostream& s)
{ s << SLL << dest << " " << src1 << " " << num << endl; }

static void emit_jalr(char *dest, ostream& s)
{ s << JALR << "\t" << dest << endl; }

static void emit_jal(const char *address,ostream &s)
{ s << JAL << address << endl; }

static void emit_return(ostream& s)
{ s << RET << endl; }

static void emit_gc_assign(ostream& s)
{ s << JAL << "_GenGC_Assign" << endl; }

static void emit_disptable_ref(Symbol sym, ostream& s)
{  s << sym << DISPTAB_SUFFIX; }

static void emit_init_ref(Symbol sym, ostream& s)
{ s << sym << CLASSINIT_SUFFIX; }

static void emit_label_ref(int l, ostream &s)
{ s << "label" << l; }

static void emit_protobj_ref(Symbol sym, ostream& s)
{ s << sym << PROTOBJ_SUFFIX; }

static void emit_method_ref(Symbol classname, Symbol methodname, ostream& s)
{ s << classname << METHOD_SEP << methodname; }

static void emit_label_def(int l, ostream &s)
{
  emit_label_ref(l,s);
  s << ":" << endl;
}

static void emit_beqz(char *source, int label, ostream &s)
{
  s << BEQZ << source << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_beq(char *src1, char *src2, int label, ostream &s)
{
  s << BEQ << src1 << " " << src2 << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_bne(char *src1, char *src2, int label, ostream &s)
{
  s << BNE << src1 << " " << src2 << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_bleq(char *src1, char *src2, int label, ostream &s)
{
  s << BLEQ << src1 << " " << src2 << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_blt(char *src1, char *src2, int label, ostream &s)
{
  s << BLT << src1 << " " << src2 << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_blti(char *src1, int imm, int label, ostream &s)
{
  s << BLT << src1 << " " << imm << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_bgti(char *src1, int imm, int label, ostream &s)
{
  s << BGT << src1 << " " << imm << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_branch(int l, ostream& s)
{
  s << BRANCH;
  emit_label_ref(l,s);
  s << endl;
}

//
// Push a register on the stack. The stack grows towards smaller addresses.
//
static void emit_push(char *reg, ostream& str)
{
  emit_store(reg,0,SP,str);
  emit_addiu(SP,SP,-4,str);
}

//
// Fetch the integer value in an Int object.
// Emits code to fetch the integer value of the Integer object pointed
// to by register source into the register dest
//
static void emit_fetch_int(char *dest, char *source, ostream& s)
{ emit_load(dest, DEFAULT_OBJFIELDS, source, s); }

//
// Emits code to store the integer value contained in register source
// into the Integer object pointed to by dest.
//
static void emit_store_int(char *source, char *dest, ostream& s)
{ emit_store(source, DEFAULT_OBJFIELDS, dest, s); }


static void emit_test_collector(ostream &s)
{
  emit_push(ACC, s);
  emit_move(ACC, SP, s); // stack end
  emit_move(A1, ZERO, s); // allocate nothing
  s << JAL << gc_collect_names[cgen_Memmgr] << endl;
  emit_addiu(SP,SP,4,s);
  emit_load(ACC,0,SP,s);
}

static void emit_gc_check(char *source, ostream &s)
{
  if (source != A1) emit_move(A1, source, s);
  s << JAL << "_gc_check" << endl;
}


///////////////////////////////////////////////////////////////////////////////
//
// coding strings, ints, and booleans
//
// Cool has three kinds of constants: strings, ints, and booleans.
// This section defines code generation for each type.
//
// All string constants are listed in the global "stringtable" and have
// type StringEntry.  StringEntry methods are defined both for String
// constant definitions and references.
//
// All integer constants are listed in the global "inttable" and have
// type IntEntry.  IntEntry methods are defined for Int
// constant definitions and references.
//
// Since there are only two Bool values, there is no need for a table.
// The two booleans are represented by instances of the class BoolConst,
// which defines the definition and reference methods for Bools.
//
///////////////////////////////////////////////////////////////////////////////

//
// Strings
//
void StringEntry::code_ref(ostream& s)
{
  s << STRCONST_PREFIX << index;
}

//
// Emit code for a constant String.
// You should fill in the code naming the dispatch table.
//

void StringEntry::code_def(ostream& s, int stringclasstag)
{
  IntEntryP lensym = inttable.add_int(len);

  // Add -1 eye catcher
  s << WORD << "-1" << endl;

  code_ref(s);  s  << LABEL                                             // label
      << WORD << stringclasstag << endl                                 // tag
      << WORD << (DEFAULT_OBJFIELDS + STRING_SLOTS + (len+4)/4) << endl // size
      << WORD;


 /***** Add dispatch information for class String ******/

  s << Str << DISPTAB_SUFFIX;

      s << endl;                                              // dispatch table
      s << WORD;  lensym->code_ref(s);  s << endl;            // string length
  emit_string_constant(s,str);                                // ascii string
  s << ALIGN;                                                 // align to word
}

//
// StrTable::code_string
// Generate a string object definition for every string constant in the 
// stringtable.
//
void StrTable::code_string_table(ostream& s, int stringclasstag)
{  
  for (List<StringEntry> *l = tbl; l; l = l->tl())
    l->hd()->code_def(s,stringclasstag);
}

//
// Ints
//
void IntEntry::code_ref(ostream &s)
{
  s << INTCONST_PREFIX << index;
}

//
// Emit code for a constant Integer.
// You should fill in the code naming the dispatch table.
//

void IntEntry::code_def(ostream &s, int intclasstag)
{
  // Add -1 eye catcher
  s << WORD << "-1" << endl;

  code_ref(s);  s << LABEL                                // label
      << WORD << intclasstag << endl                      // class tag
      << WORD << (DEFAULT_OBJFIELDS + INT_SLOTS) << endl  // object size
      << WORD; 

 /***** Add dispatch information for class Int ******/
      s << Int << DISPTAB_SUFFIX;

      s << endl;                                          // dispatch table
      s << WORD << str << endl;                           // integer value
}


//
// IntTable::code_string_table
// Generate an Int object definition for every Int constant in the
// inttable.
//
void IntTable::code_string_table(ostream &s, int intclasstag)
{
  for (List<IntEntry> *l = tbl; l; l = l->tl())
    l->hd()->code_def(s,intclasstag);
}


//
// Bools
//
BoolConst::BoolConst(int i) : val(i) { assert(i == 0 || i == 1); }

void BoolConst::code_ref(ostream& s) const
{
  s << BOOLCONST_PREFIX << val;
}
  
//
// Emit code for a constant Bool.
// You should fill in the code naming the dispatch table.
//

void BoolConst::code_def(ostream& s, int boolclasstag)
{
  // Add -1 eye catcher
  s << WORD << "-1" << endl;

  code_ref(s);  s << LABEL                                  // label
      << WORD << boolclasstag << endl                       // class tag
      << WORD << (DEFAULT_OBJFIELDS + BOOL_SLOTS) << endl   // object size
      << WORD;

 /***** Add dispatch information for class Bool ******/
      s << Bool << DISPTAB_SUFFIX;

      s << endl;                                            // dispatch table
      s << WORD << val << endl;                             // value (0 or 1)
}

//////////////////////////////////////////////////////////////////////////////
//
//  CgenClassTable methods
//
//////////////////////////////////////////////////////////////////////////////

//***************************************************
//
//  Emit code to start the .data segment and to
//  declare the global names.
//
//***************************************************

void CgenClassTable::code_global_data()
{
  Symbol main    = idtable.lookup_string(MAINNAME);
  Symbol string  = idtable.lookup_string(STRINGNAME);
  Symbol integer = idtable.lookup_string(INTNAME);
  Symbol boolc   = idtable.lookup_string(BOOLNAME);

  str << "\t.data\n" << ALIGN;
  //
  // The following global names must be defined first.
  //
  str << GLOBAL << CLASSNAMETAB << endl;
  str << GLOBAL; emit_protobj_ref(main,str);    str << endl;
  str << GLOBAL; emit_protobj_ref(integer,str); str << endl;
  str << GLOBAL; emit_protobj_ref(string,str);  str << endl;
  str << GLOBAL; falsebool.code_ref(str);  str << endl;
  str << GLOBAL; truebool.code_ref(str);   str << endl;
  str << GLOBAL << INTTAG << endl;
  str << GLOBAL << BOOLTAG << endl;
  str << GLOBAL << STRINGTAG << endl;

  //
  // We also need to know the tag of the Int, String, and Bool classes
  // during code generation.
  //
  str << INTTAG << LABEL
      << WORD << intclasstag << endl;
  str << BOOLTAG << LABEL 
      << WORD << boolclasstag << endl;
  str << STRINGTAG << LABEL 
      << WORD << stringclasstag << endl;    
}


//***************************************************
//
//  Emit code to start the .text segment and to
//  declare the global names.
//
//***************************************************

void CgenClassTable::code_global_text()
{
  str << GLOBAL << HEAP_START << endl
      << HEAP_START << LABEL 
      << WORD << 0 << endl
      << "\t.text" << endl
      << GLOBAL;
  emit_init_ref(idtable.add_string("Main"), str);
  str << endl << GLOBAL;
  emit_init_ref(idtable.add_string("Int"),str);
  str << endl << GLOBAL;
  emit_init_ref(idtable.add_string("String"),str);
  str << endl << GLOBAL;
  emit_init_ref(idtable.add_string("Bool"),str);
  str << endl << GLOBAL;
  emit_method_ref(idtable.add_string("Main"), idtable.add_string("main"), str);
  str << endl;
}

void CgenClassTable::code_bools(int boolclasstag)
{
  falsebool.code_def(str,boolclasstag);
  truebool.code_def(str,boolclasstag);
}

void CgenClassTable::code_select_gc()
{
  //
  // Generate GC choice constants (pointers to GC functions)
  //
  str << GLOBAL << "_MemMgr_INITIALIZER" << endl;
  str << "_MemMgr_INITIALIZER:" << endl;
  str << WORD << gc_init_names[cgen_Memmgr] << endl;
  str << GLOBAL << "_MemMgr_COLLECTOR" << endl;
  str << "_MemMgr_COLLECTOR:" << endl;
  str << WORD << gc_collect_names[cgen_Memmgr] << endl;
  str << GLOBAL << "_MemMgr_TEST" << endl;
  str << "_MemMgr_TEST:" << endl;
  str << WORD << (cgen_Memmgr_Test == GC_TEST) << endl;
}


//********************************************************
//
// Emit code to reserve space for and initialize all of
// the constants.  Class names should have been added to
// the string table (in the supplied code, is is done
// during the construction of the inheritance graph), and
// code for emitting string constants as a side effect adds
// the string's length to the integer table.  The constants
// are emmitted by running through the stringtable and inttable
// and producing code for each entry.
//
//********************************************************

void CgenClassTable::code_constants()
{
  //
  // Add constants that are required by the code generator.
  //
  stringtable.add_string("");
  inttable.add_string("0");

  stringtable.code_string_table(str,stringclasstag);
  inttable.code_string_table(str,intclasstag);
  code_bools(boolclasstag);
}


CgenClassTable::CgenClassTable(Classes classes, ostream& s) : nds(NULL) , str(s)
{
   stringclasstag = 1 /* Change to your String class tag here */;
   intclasstag =    2 /* Change to your Int class tag here */;
   boolclasstag =   3 /* Change to your Bool class tag here */;

   enterscope();
   install_basic_classes();
   install_classes(classes);
   build_inheritance_tree();
   code();
   exitscope();
}

void CgenClassTable::install_basic_classes()
{

// The tree package uses these globals to annotate the classes built below.
  curr_lineno  = 0;
  Symbol filename = stringtable.add_string("<basic class>");

//
// A few special class names are installed in the lookup table but not
// the class list.  Thus, these classes exist, but are not part of the
// inheritance hierarchy.
// No_class serves as the parent of Object and the other special classes.
// SELF_TYPE is the self class; it cannot be redefined or inherited.
// prim_slot is a class known to the code generator.
//
  addid(No_class,
	new CgenNode(class_(No_class,No_class,nil_Features(),filename),
			    Basic,this));
  addid(SELF_TYPE,
	new CgenNode(class_(SELF_TYPE,No_class,nil_Features(),filename),
			    Basic,this));
  addid(prim_slot,
	new CgenNode(class_(prim_slot,No_class,nil_Features(),filename),
			    Basic,this));

// 
// The Object class has no parent class. Its methods are
//        cool_abort() : Object    aborts the program
//        type_name() : Str        returns a string representation of class name
//        copy() : SELF_TYPE       returns a copy of the object
//
// There is no need for method bodies in the basic classes---these
// are already built in to the runtime system.
//
  install_class(
   new CgenNode(
    class_(Object, 
	   No_class,
	   append_Features(
           append_Features(
           single_Features(method(cool_abort, nil_Formals(), Object, no_expr())),
           single_Features(method(type_name, nil_Formals(), Str, no_expr()))),
           single_Features(method(copy, nil_Formals(), SELF_TYPE, no_expr()))),
	   filename),
    Basic,this));

// 
// The IO class inherits from Object. Its methods are
//        out_string(Str) : SELF_TYPE          writes a string to the output
//        out_int(Int) : SELF_TYPE               "    an int    "  "     "
//        in_string() : Str                    reads a string from the input
//        in_int() : Int                         "   an int     "  "     "
//
   install_class(
    new CgenNode(
     class_(IO, 
            Object,
            append_Features(
            append_Features(
            append_Features(
            single_Features(method(out_string, single_Formals(formal(arg, Str)),
                        SELF_TYPE, no_expr())),
            single_Features(method(out_int, single_Formals(formal(arg, Int)),
                        SELF_TYPE, no_expr()))),
            single_Features(method(in_string, nil_Formals(), Str, no_expr()))),
            single_Features(method(in_int, nil_Formals(), Int, no_expr()))),
	   filename),	    
    Basic,this));

//
// The Int class has no methods and only a single attribute, the
// "val" for the integer. 
//
   install_class(
    new CgenNode(
     class_(Int, 
	    Object,
            single_Features(attr(val, prim_slot, no_expr())),
	    filename),
     Basic,this));

//
// Bool also has only the "val" slot.
//
    install_class(
     new CgenNode(
      class_(Bool, Object, single_Features(attr(val, prim_slot, no_expr())),filename),
      Basic,this));

//
// The class Str has a number of slots and operations:
//       val                                  ???
//       str_field                            the string itself
//       length() : Int                       length of the string
//       concat(arg: Str) : Str               string concatenation
//       substr(arg: Int, arg2: Int): Str     substring
//       
   install_class(
    new CgenNode(
      class_(Str, 
	     Object,
             append_Features(
             append_Features(
             append_Features(
             append_Features(
             single_Features(attr(val, Int, no_expr())),
            single_Features(attr(str_field, prim_slot, no_expr()))),
            single_Features(method(length, nil_Formals(), Int, no_expr()))),
            single_Features(method(concat, 
				   single_Formals(formal(arg, Str)),
				   Str, 
				   no_expr()))),
	    single_Features(method(substr, 
				   append_Formals(single_Formals(formal(arg, Int)), 
						  single_Formals(formal(arg2, Int))),
				   Str, 
				   no_expr()))),
	     filename),
        Basic,this));

}

// CgenClassTable::install_class
// CgenClassTable::install_classes
//
// install_classes enters a list of classes in the symbol table.
//
void CgenClassTable::install_class(CgenNodeP nd)
{
  Symbol name = nd->get_name();

  if (probe(name))
    {
      return;
    }

  // The class name is legal, so add it to the list of classes
  // and the symbol table.
  nds = new List<CgenNode>(nd,nds);
  addid(name,nd);
}

void CgenClassTable::install_classes(Classes cs)
{
  for(int i = cs->first(); cs->more(i); i = cs->next(i))
    install_class(new CgenNode(cs->nth(i),NotBasic,this));
}

//
// CgenClassTable::build_inheritance_tree
//
void CgenClassTable::build_inheritance_tree()
{
  for(List<CgenNode> *l = nds; l; l = l->tl())
      set_relations(l->hd());
}

//
// CgenClassTable::set_relations
//
// Takes a CgenNode and locates its, and its parent's, inheritance nodes
// via the class table.  Parent and child pointers are added as appropriate.
//
void CgenClassTable::set_relations(CgenNodeP nd)
{
  CgenNode *parent_node = probe(nd->get_parent());
  nd->set_parentnd(parent_node);
  parent_node->add_child(nd);
}

void CgenNode::add_child(CgenNodeP n)
{
  children = new List<CgenNode>(n,children);
}

void CgenNode::set_parentnd(CgenNodeP p)
{
  assert(parentnd == NULL);
  assert(p != NULL);
  parentnd = p;
}



void CgenClassTable::code()
{
  if (cgen_debug) cout << "coding global data" << endl;
  code_global_data();

  if (cgen_debug) cout << "choosing gc" << endl;
  code_select_gc();

  if (cgen_debug) cout << "coding constants" << endl;
  code_constants();

  if (cgen_debug) cout << "coding prototype objects" << endl;
  code_prototypeObjects();

  if (cgen_debug) cout << "coding name table" << endl;
  code_classNameTable();

  // Tabela criada para possibilitar o uso do SELF_TYPE
  // Essa tabela possui uma referência a cada prototype de um objeto
  // Dada sua classtag
  if (cgen_debug) cout << "coding class prototype table" << endl;
  code_classPrototypeTable();

  if (cgen_debug) cout << "coding dispatch tables" << endl;
  code_dispatchTables();

  if (cgen_debug) cout << "coding global text" << endl;
  code_global_text();

  if (cgen_debug) cout << "coding object initializer" << endl;
  code_objectInitializer();

  if (cgen_debug) cout << "coding class methods" << endl;
  code_classMethods();

}

std::vector<CgenNode*> CgenClassTable::getClassNodes() {
  auto nodeList = this->nds;
  std::vector<CgenNode*> classNodes;
  // Salva os nós de todas as classes
  for(List<CgenNode>* list = nodeList; list != nullptr; list = list->tl()) {
    auto node = list->hd();
    classNodes.push_back(node);
  }
  return classNodes;
}

void CgenClassTable::code_classPrototypeTable() {
  str << "classPrototypeTable" << LABEL;
  auto classes = this->getClassNodes();
  // Cria a tabela sendo que para cada classe temos duas entradas
  // A primeira indica o prototype e a segunda indica o init
  for(auto classNode : classes) {
    str << WORD << classNode->get_name() << PROTOBJ_SUFFIX << endl;
    str << WORD << classNode->get_name() << CLASSINIT_SUFFIX << endl;
  }
}

void CgenClassTable::code_classMethods() {
  auto classes = this->getClassNodes();
  for(auto classNode : classes) {
    // Se a classe não é Int, Bool nem String
    if(!classNode->basic()) {
      currentClass = classNode;
      classNode->code_classMethods(str);
    } 
  }
  currentClass = classes[0];
}

std::map<Symbol, std::pair<method_class*, Symbol> > CgenNode::getFunctionsOfClass() {
  auto currentNode = this;
  auto parent = this->get_parentnd();
  // Map para salvar a classe a qual cada função pertence, além do tipo de retorno da função
  std::map<Symbol, std::pair<method_class*, Symbol> > functionClassMap;
  while(currentNode != nullptr) {
    auto features = currentNode->features;
    for(int it = features->first(); features->more(it); it = features->next(it)) {
      auto feature = features->nth(it);
      // Feature é do tipo método
      if(feature->getType() == 1) {
        method_class* method = (method_class*)feature;
        if(functionClassMap.find(method->getName()) == functionClassMap.end()) {
          functionClassMap[method->getName()] = {method, currentNode->get_name()};
        }
      }
    }
    auto temp = currentNode->get_parentnd();
    parent = currentNode->get_parentnd();
    currentNode = temp;
  }
  return functionClassMap;
}

void CgenNode::code_classMethods(ostream& str) {
  // Mapa de função para objeto method_class e símbolo da classe da função
  auto functionMap = getFunctionsOfClass();
  for(auto function : functionMap) {
    if(function.second.second != this->get_name()) continue;
    method_class* method = function.second.first;
    str << this->get_name() << METHOD_SEP << method->getName() << LABEL;
    
    // Executa um PUSH de fp, s0 e ra na pilha
    emit_push(FP, str);
    emit_push(SELF, str);
    emit_push(RA, str);
    elementsInStack+=3;
    
    // Coloca o fp para apontar para o endereço de retorno na pilha
    emit_addiu(FP, SP, 4, str);

    // Salva a0 no self
    emit_move(SELF, ACC, str);

    // Coloca os parâmetros na pilha

    // Avalia a expressão do corpo do método
    method->getBody()->code(str);

    // Executa um POP de fp, s0 e ra na pilha
    emit_load(FP, 3, SP, str);
    emit_load(SELF, 2, SP, str);
    emit_load(RA, 1, SP, str);
    emit_addiu(SP, SP, 3*WORD_SIZE, str);
    elementsInStack-=3;

    // Retorna do método
    emit_return(str);
  }
}

void CgenClassTable::code_objectInitializer() {
  auto classes = this->getClassNodes();
  for(auto classNode : classes) {
    currentClass = classNode;
    classNode->code_objectInitializer(str);
  }
  currentClass = classes[0];
}

void CgenNode::code_attributeInitializer(ostream& str) {
  // Inicializa os atributos da classe
  auto features = this->features;
  for(int it = features->first(); features->more(it); it = features->next(it)) {
    auto feature = features->nth(it);
    // Feature é do tipo atributo
    if(feature->getType() == 0) {
      attr_class* attribute = (attr_class*)feature;
      // Obtem o offset do atributo no prototype
      int offset = offsetClassAttr[this->get_name()][attribute->getName()];
      // Se o atributo não possui inicialização
      if(attribute->getInit()->isNoExpr()) {
        // Carrega o valor padrão da inicialização de cada tipo no a0
        if(attribute->type_decl == Bool) {
          emit_load_bool(ACC, BoolConst(0), str);
        } else if(attribute->type_decl == Int) {
          IntEntry* zeroEntry = inttable.lookup_string("0");
          emit_load_int(ACC, zeroEntry, str);
        } else if(attribute->type_decl == Str) {
          StringEntry* emptyEntry = stringtable.lookup_string("");
          emit_load_string(ACC, emptyEntry, str);
        }
        // Salva o valor carregado na posição correta da memória
        emit_store(ACC, DEFAULT_OBJFIELDS + offset, SELF, str);
      } else {
        // O atributo possui inicialização
        // Carrega o resultado do código de uma expression no a0
        attribute->getInit()->code(str);
        // Salva o resultado de a0 na posição correta
        emit_store(ACC, DEFAULT_OBJFIELDS + offset, SELF, str);
        // Gera informações para o coletor de lixo, caso ele esteja ativo
        if(cgen_Memmgr) {
          emit_addiu(A1, SELF, 4*(offset + DEFAULT_OBJFIELDS), str);
          emit_jal("_GenGC_Assign", str);
        }
      }
    }
  }
}

void CgenNode::code_objectInitializer(ostream& str) {
  str << this->get_name() << CLASSINIT_SUFFIX << LABEL;
  // Executa PUSH de fp, seguido de s0, seguido de ra
  emit_push(FP, str);
  emit_push(SELF, str);
  emit_push(RA, str);
  elementsInStack+=3;
  // Faz com que fp aponte para o endereço de retorno na pilha
  emit_addiu(FP, SP, 4, str);
  // Faz com que o registrador self seja igual a a0, pois como explicado
  // no cool-runtime, o objeto self é passado no registrador a0
  emit_move(SELF, ACC, str);
  // Roda o inicializar na classe mãe
  // Se o nó for o Object
  if(this->get_parentnd()->name != No_class) {
    str << JAL;
    emit_init_ref(this->get_parentnd()->get_name(), str);
    str << endl;
  }
  // Inicializa os atributos
  this->code_attributeInitializer(str);
  // Retorna da inicialização
  // Salva o self no a0
  emit_move(ACC, SELF, str);
  // Executa POP de fp, seguido de s0, seguido de ra
  emit_load(FP, 3, SP, str);
  elementsInStack-=3;
  emit_load(SELF, 2, SP, str);
  emit_load(RA, 1, SP, str);
  emit_addiu(SP, SP, 3*WORD_SIZE, str);
  // Executa o retorno da inicialização
  emit_return(str);
}

void CgenClassTable::code_dispatchTables() {
  auto classes = this->getClassNodes();
  for(auto classNode : classes) {
    currentClass = classNode;
    classNode->code_dispatchTable(str);
  }
}

void CgenNode::code_dispatchTable(ostream& str) {
  if(cgen_debug) {
    cout << "Generating dispatch table of class " << this->get_name() << endl;
  }
  str << this->get_name() << DISPTAB_SUFFIX << LABEL;
  // Map para salvar a classe a qual cada função pertence, além do tipo de retorno da função
  auto functionClassMap = this->getFunctionsOfClass();
  int offset = 0;
  for(auto functionClass : functionClassMap) {
    str << WORD << functionClass.second.second << METHOD_SEP << functionClass.first << endl;
    methodOffsetClassMethod[this->get_name()][functionClass.first] = {functionClass.second.first, {offset++, functionClass.second.second}};
  }
}

void CgenClassTable::code_classNameTable() {
  // Adiciona o "eyecatcher"
  str << CLASSNAMETAB << LABEL;
  auto classes = this->getClassNodes();
  // Coloca na mesma ordem do coolc
  std::reverse(classes.begin(), classes.end());
  for(auto classNode : classes) {
    StringEntry* stringEntry = stringtable.lookup_string(classNode->get_name()->get_string());
    str << WORD;
    stringEntry->code_ref(str);
    str << endl;
  }
}

int CgenNode::getSize() {
  int sizeClass = 0;
  auto features = this->features;
  for(int it = features->first(); features->more(it); it = features->next(it)) {
    auto feature = features->nth(it);
    // Feature é do tipo atributo
    if(feature->getType() == 0) {
      sizeClass++;
    }
  }

  // Soma o tamanho da classe com o tamanho da classe pai
  if(this->get_parentnd() != nullptr) {
    sizeClass += this->get_parentnd()->getSize();
  }

  return sizeClass;
}

std::vector<std::pair<CgenNode*, std::pair<int, int> > > CgenClassTable::getClassNodeTagAndSize() {
  auto nodes = getClassNodes();
  int counterTag = 4;
  std::vector<std::pair<CgenNode*, std::pair<int, int> > > namesTagsSize;
  // Salva a tag, o nó e tamanho de cada classe
  for(CgenNode* node : nodes) {
    if(!node->basic()) {
      int tag = counterTag++;
      // Salva a classe dada pela tag atual
      classesByTag[tag] = node;
      int size = node->getSize();
      namesTagsSize.push_back({node, {tag, size}});
    } else {
      if(node->name == Bool) {
        int size = node->getSize();
        namesTagsSize.push_back({node, {3, size}});
        // Salva a classe dada pela tag atual
        classesByTag[3] = node;
      } else if(node->name == Int) {
        int size = node->getSize();
        namesTagsSize.push_back({node, {2, size}});
        // Salva a classe dada pela tag atual
        classesByTag[2] = node;
      } else { // String
        int size = node->getSize();
        namesTagsSize.push_back({node, {1, size}});
        // Salva a classe dada pela tag atual
        classesByTag[1] = node;
      }
    }
  }
  return namesTagsSize;
}

void CgenClassTable::code_prototypeObjects() {
  auto classes = this->getClassNodeTagAndSize();
  // Chama a geração de código do nó da classe
  for(auto classNode : classes) {
    classNodeMap[classNode.first->get_name()] = classNode.first;
    classNode.first->code_prototypeObject(this->str, classNode.second.first, classNode.second.second);
  }
}

void CgenNode::code_prototypeObject(ostream& str, int tag, int size) {
  if(cgen_debug) {
    cout << "Generating class code " << this->get_name() << endl;
  }
  // Adiciona o "eyecatcher"
  str << WORD << "-1" << endl;
  // Adiciona a tag do prototype
  str << this->get_name() << PROTOBJ_SUFFIX << LABEL;
  // Adiciona a tag da classe
  str << WORD << tag << endl;
  // Adiciona o tamanho da classe
  str << WORD <<  DEFAULT_OBJFIELDS + size << endl;
  // Adiciona o ponteiro para a tabela de dispatch
  str << WORD << this->get_name() << DISPTAB_SUFFIX << endl;
  // Adiciona os atributos, com offset inicial igual a 3, pois já existem outros
  // 3 campos de objetos padrões
  int offset = DEFAULT_OBJFIELDS;
  this->code_attributesPrototypeObject(str, offset);
}

void CgenNode::code_attributesPrototypeObject(ostream& str, int& offset) {
  // Gera os atributos do pai, caso tenha
  if(this->get_parentnd() != nullptr) {
    this->get_parentnd() -> code_attributesPrototypeObject(str, offset);
  }

  auto features = this->features;
  for(int it = features->first(); features->more(it); it = features->next(it)) {
    auto feature = features->nth(it);
    // Feature é do tipo atributo
    if(feature->getType() == 0) {
      attr_class* attribute = (attr_class*)feature;
      auto typeAttribute = attribute->type_decl;
      // Salva o tipo e o offset de cada atributo
      offsetClassAttr[this->get_name()][attribute->getName()] = offset++;
      // Gera o código que inicializa os atributos
      if(typeAttribute == Str) {
        str << WORD;
        stringtable.lookup_string("")->code_ref(str);
        str << endl;
      }
      else if(typeAttribute == Int) {
        IntEntryP entradaInt = inttable.add_int(0);
        str << WORD;
        entradaInt->code_ref(str);
        str << endl;
      }
      else if(typeAttribute == Bool) {
        str << WORD;
        falsebool.code_ref(str);
        str << endl;
      } else {
        str << WORD << 0 << endl;
      }
    }
  }
}

CgenNodeP CgenClassTable::root()
{
   return probe(Object);
}


///////////////////////////////////////////////////////////////////////
//
// CgenNode methods
//
///////////////////////////////////////////////////////////////////////

CgenNode::CgenNode(Class_ nd, Basicness bstatus, CgenClassTableP ct) :
   class__class((const class__class &) *nd),
   parentnd(NULL),
   children(NULL),
   basic_status(bstatus)
{ 
   stringtable.add_string(name->get_string());          // Add class name to string table
}


//******************************************************************
//
//   Fill in the following methods to produce code for the
//   appropriate expression.  You may add or remove parameters
//   as you wish, but if you do, remember to change the parameters
//   of the declarations in `cool-tree.h'  Sample code for
//   constant integers, strings, and booleans are provided.
//
//*****************************************************************

int getStackOffset(Symbol name) {
  int stackIndex = symbolTable[name].back();
  // Calcula o offset com base no numero de elementos na pilha
  // e o índice do símbolo
  int offset = (elementsInStack-1-stackIndex);
  return offset;
}

void assign_class::code(ostream &s) {
  // Avalia a expressão a ser atribuída
  this->expr->code(s);
  if(symbolTable.find(name) == symbolTable.end() || symbolTable[name].back() < scopes.back()) {
    s << "ERRO" << endl;
    return;
  }
  // Busca a variável para ser alterada
  int offset = getStackOffset(this->name);
  emit_store(ACC, offset, SP, s);
  // Tratamento do garbage collector
  if(cgen_Memmgr) {
    emit_addiu(A1, FP, 4*(offset), s);
    emit_jal("_GenGC_Assign", s);
  }
}

std::vector<int> loadAttributesInStack(Symbol caleeType, ostream& s) {
  std::vector<int> ret;
  // Atributos da classe
  auto mapAttrTypeOffset = offsetClassAttr[caleeType];
  for(auto attrTypeOffset : mapAttrTypeOffset) {
    // Salva em T1 o endereço do atributo do objeto na memória
    // Lembrando que ACC contém um ponteiro para o objeto
    emit_addiu(T1, ACC, attrTypeOffset.second, s);
    // Coloca na pilha o endereço do atributo
    emit_push(T1, s);
    // Salva o atributo na tabela de símbolos
    symbolTable[attrTypeOffset.first].push_back(elementsInStack);
    reverseSymbolTable[elementsInStack] = attrTypeOffset.first;
    ret.push_back(elementsInStack);
    elementsInStack++;
  }
  return ret;
}

std::vector<int> loadParametersInStack(Expressions actuals, Symbol methodSymbol, ostream& s) {
  std::vector<int> ret;
  // Percorre os parâmetros
  for(int it = actuals->first(); actuals->more(it); it = actuals->next(it)) {
    auto expression = actuals->nth(it);
    // Avalia o parâmetro
    expression->code(s);
    // Coloca seu resultado na pilha
    emit_push(ACC, s);
    // Salva o parâmetro na tabela de símbolos
    if(cgen_debug) {cout << currentClass->get_name() << methodSymbol << endl; }
    auto methodOffset = methodOffsetClassMethod[currentClass->get_name()][methodSymbol];
    Symbol parameterName = methodOffset.first->formals->nth(it)->getName();
    symbolTable[parameterName].push_back(elementsInStack);
    reverseSymbolTable[elementsInStack]= parameterName;
    ret.push_back(elementsInStack);
    elementsInStack++;
  }
  return ret;
}

void unloadDataInStack(std::vector<int> indexes, ostream& s) {
  // Remove a variável da pilha e da tabela de símbolos
  for(int index : indexes) {
    emit_addiu(SP, SP, WORD_SIZE, s);
    elementsInStack--;
    symbolTable[reverseSymbolTable[index]].pop_back();
  }
}

int labelId = 0;

void static_dispatch_class::code(ostream &s) {
  // Cria um novo escopo para o novo método chamado
  // Esse escopo começa com os parâmetros do método
  scopes.push_back(elementsInStack);
  // Carrega o offset do método chamado
  int offset = methodOffsetClassMethod[this->type_name][this->name].second.first;
  Symbol className = methodOffsetClassMethod[this->type_name][this->name].second.second;
  // Atualiza currentClass
  auto oldClass = currentClass;
  currentClass = classNodeMap[this->type_name];
   // Carregar atributos do objeto na memória
  auto addedAttributesIndexes = loadAttributesInStack(expr->get_type(), s);
  // Percorre os parâmetros e os adiciona na pilha
  auto addedParametersIndexes = loadParametersInStack(this->actual, this->name, s);
  // Avalia o objeto
  expr->code(s);
  // Verifica se o objeto é void
  emit_bne(ACC, ZERO, labelId, s);
  // Carrega o nome do programa em a0
  emit_load_address(ACC, "str_const0", s);
  // Carrega a linha no registrador T1
  emit_load_imm(T1, this->line_number, s);
  // Aborta com o procedimento correto
  emit_jal("_dispatch_abort", s);
  // Coloca o label para o objeto diferente de void
  emit_label_def(labelId++, s);
  // Carrega o endereço do método na tabela de dispatch
  std::string address = className->get_string();
  address+= METHOD_SEP;
  address+= this->name->get_string();
  emit_load_address(T1, address.c_str(), s);
  // Chama o método
  emit_jalr(T1, s);
  // Atualiza currentClass
  currentClass = oldClass;
  // Remove os parâmetros e novos atributos do estado e volta ao escopo antigo
  scopes.pop_back();
  if(address != "IO.out_string" || address != "IO.out_int") {
    // Remove os parâmetros da pilha
    unloadDataInStack(addedParametersIndexes, s);
  }
  // Remove os atributos da pilha
  unloadDataInStack(addedAttributesIndexes, s);
}

void dispatch_class::code(ostream &s) {
  // Cria um novo escopo para o novo método chamado
  // Esse escopo começa com os parâmetros do método
  scopes.push_back(elementsInStack);
  // Verifica se o método chamado é de um objeto ou da classe atual
  Symbol className = currentClass->get_name();
  if(expr->get_type() != SELF_TYPE) {
    className = expr->get_type();
  }
  // Carrega o offset do método chamado
  int offset = methodOffsetClassMethod[className][this->name].second.first;
  Symbol methodClassName = methodOffsetClassMethod[className][this->name].second.second;
  // Atualiza currentClass
  auto oldClass = currentClass;
  currentClass = classNodeMap[className];
  // Carregar atributos do objeto na memória
  auto addedAttributesIndexes = loadAttributesInStack(expr->get_type(), s);
  // Percorre os parâmetros e os adiciona na pilha
  auto addedParametersIndexes = loadParametersInStack(this->actual, this->name, s);
  // Avalia o objeto
  expr->code(s);
  // Verifica se o objeto é void
  emit_bne(ACC, ZERO, labelId, s);
  // Carrega o nome do programa em a0
  emit_load_address(ACC, "str_const0", s);
  // Carrega a linha no registrador T1
  emit_load_imm(T1, this->line_number, s);
  // Aborta com o procedimento correto
  emit_jal("_dispatch_abort", s);
  // Coloca o label para o objeto diferente de void
  emit_label_def(labelId++, s);
  // Carrega o endereço do método na tabela de dispatch
  std::string address = methodClassName->get_string();
  address+= METHOD_SEP;
  address+= this->name->get_string();
  emit_load_address(T1, address.c_str(), s);
  // Chama o método
  emit_jalr(T1, s);
  // Atualiza currentClass
  currentClass = oldClass;
  // Remove os parâmetros e novos atributos do estado e volta ao escopo antigo
  scopes.pop_back();
  // Essas funções dão pop no argumento dentro delas
  if(address != "IO.out_string" || address != "IO.out_int") {
    // Remove os parâmetros da pilha
    unloadDataInStack(addedParametersIndexes, s);
  }
  // Remove os atributos da pilha
  unloadDataInStack(addedAttributesIndexes, s);
}

void cond_class::code(ostream &s) {
  int labelFalse = labelId++;
  int labelEndIf = labelId++;
  int labelTrue = labelId++;
  this->pred->code(s);
  // Carrega o valor do objeto Bool retornado
  emit_load(ACC, 3, ACC, s);
  // Se ele for true, vai para o then
  emit_bne(ACC, ZERO, labelTrue, s);
  // Condição falsa
  emit_label_def(labelFalse, s);
  // Trecho do false
  this->else_exp->code(s);
  // Vai para o fim do if
  emit_branch(labelEndIf, s);
  // Trecho do true
  emit_label_def(labelTrue, s);
  this->then_exp->code(s);
  // Define o fim do if
  emit_label_def(labelEndIf, s);
} 

void loop_class::code(ostream &s) {
  int labelWhile = labelId++;
  int labelBreak = labelId++;
  emit_label_def(labelWhile, s);
  // Testa a condição do while
  this->pred->code(s);
  // Carrega o resultado
  emit_load(ACC, 3, ACC, s);
  // Sai do while se o resultado da condição for false
  emit_beqz(ACC, labelBreak, s);
  // Executa o corpo do while
  this->body->code(s);
  // Repete o loop
  emit_branch(labelWhile, s);
  // Saída do loop
  emit_label_def(labelBreak, s);
  // Retorna void para evitar de retornar o resultado da última execução do corpo do while
  emit_move(ACC, ZERO, s);
}

void typcase_class::code(ostream &s) {
  // Avalia a expressão do case
  this->expr->code(s);
  int labelExprValida = labelId++;
  // Se a expressão do case for void, aborta chamando o procedimento correto
  emit_bne(ACC, ZERO, labelExprValida, s);
  // Carrega o nome do arquivo no a0
  emit_load_address(ACC, "str_const0", s);
  // Carrega a linha no registrador t1
  emit_load_imm(T1, this->line_number, s);
  // Chama o procedimento para abortar
  emit_jal("_case_abort2", s);
  // Define a label da expressão válida
  emit_label_def(labelExprValida, s);
  // Carrega o objeto em T1
  emit_load(T1, 0, ACC, s);
  // Código para checar os cases
  std::vector<std::pair<int, branch_class*> > labelsMatches;
  for (int it = this->cases->first(); this->cases->more(it); it = this->cases->next(it)) {
    auto caseObj = this->cases->nth(it);
    branch_class* branch = (branch_class*) caseObj;
    // Faz o match da expressão com o tipo do case
    // Carrega em T2 a tag da classe desse match
    emit_load_address(T2, branch->type_decl->get_string(), s);
    // Se as tags do match e da expressão forem iguais, vai para o trecho
    // que executa o código do match
    emit_beq(T1, T2, labelId, s);
    labelsMatches.push_back({labelId++, branch});
  }
  int labelEndCaseWithMatch = labelId++;
  for(auto labelMatch : labelsMatches) {
    emit_label_def(labelMatch.first, s);
    // Adiciona a variável do match na pilha e na tabela de símbolos
    // Copia o objeto resultado da expressão do match (que está em a0)
    emit_jal("Object.copy", s);
    // Salva o resultado da cópia na pilha e na tabela de símbolos
    int indexVariable = elementsInStack;
    emit_push(ACC, s);
    symbolTable[labelMatch.second->name].push_back(elementsInStack);
    reverseSymbolTable[elementsInStack] = labelMatch.second->name;
    elementsInStack++;
    labelMatch.second->expr->code(s);
    // Remove o resultado da cópia da pilha
    emit_addiu(SP, SP, WORD_SIZE, s);
    elementsInStack--;
    symbolTable[reverseSymbolTable[indexVariable]].pop_back();
    // Vai para a label de fim do case
    emit_branch(labelEndCaseWithMatch, s);
  }
  // Executa o procedimento para abortar o case caso não tenha match
  // Nesse caso o ACC já contém o tipo da expressão do case
  emit_jal("_case_abort", s);
  // Define a label que identifica um case que finalizou corretamente
  emit_label_def(labelEndCaseWithMatch, s);
}

void block_class::code(ostream &s) {
  // Gera o código de todos elementos do corpo, retornando o último
  for (int it = this->body->first(); this->body->more(it); it = this->body->next(it)) {
    this->body->nth(it)->code(s);
  }
}

void let_class::code(ostream &s) {
  // Avalia o valor de inicialização da variável
  this->init->code(s);
  if(this->init->isNoExpr()) {
    // Carrega os valores padrões caso seja um tipo básico
    if(this->type_decl == Int) {
      emit_load_int(ACC, inttable.lookup_string("0"), s);
    } else if (this->type_decl == Bool) {
      emit_load_bool(ACC, BoolConst(0), s);
    } else { // String
      emit_load_string(ACC, stringtable.lookup_string(""), s);
    }
  }
  // Coloca o identificador do let na pilha e na tabela de símbolos
  int indexVariable = elementsInStack;
  emit_push(ACC, s);
  symbolTable[this->identifier].push_back(elementsInStack);
  reverseSymbolTable[elementsInStack] = this->identifier;
  elementsInStack++;
  // Avalia a expressão do let
  this->body->code(s);
  // Remove o identificador da pilha e da tabela de símbolos
  emit_addiu(SP, SP, WORD_SIZE, s);
  elementsInStack--;
  symbolTable[reverseSymbolTable[indexVariable]].pop_back();
}

void plus_class::code(ostream &s) {
  // Avalia e1
  this->e1->code(s);
  // Salva e1 na pilha
  emit_push(ACC, s);
  elementsInStack++;

  // Avalia e2
  this->e2->code(s);
  // Copia o objeto resultado de e2 para salvar o resultado
  emit_jal("Object.copy", s);
  // Remove e1 da pilha
  emit_addiu(SP, SP, WORD_SIZE, s);
  elementsInStack--;
  // Salva e1 em T1
  emit_load(T1, 0, SP, s);
  // Salva a cópia de e2 em T2
  emit_move(T2, ACC, s);
  // Carrega os valores inteiros dos objetos
  emit_load(T1, 3, T1, s);
  emit_load(T2, 3, T2, s);
  // Salva a soma em T3
  emit_add(T3, T1, T2, s);
  // Salva no valor inteiro da cópia de e2 o valor de T3
  // A cópia de e2 está no registrador a0
  emit_store(T3, 3, ACC, s);
}

void sub_class::code(ostream &s) {
  // Avalia e1
  this->e1->code(s);
  // Salva e1 na pilha
  emit_push(ACC, s);
  elementsInStack++;

  // Avalia e2
  this->e2->code(s);
  // Copia o objeto resultado de e2 para salvar o resultado
  emit_jal("Object.copy", s);
  // Remove e1 da pilha
  emit_addiu(SP, SP, WORD_SIZE, s);
  elementsInStack--;
  // Salva e1 em T1
  emit_load(T1, 0, SP, s);
  // Salva a cópia de e2 em T2
  emit_move(T2, ACC, s);
  // Carrega os valores inteiros dos objetos
  emit_load(T1, 3, T1, s);
  emit_load(T2, 3, T2, s);
  // Salva a subtração em T3
  emit_sub(T3, T1, T2, s);
  // Salva no valor inteiro da cópia de e2 o valor de T3
  // A cópia de e2 está no registrador a0
  emit_store(T3, 3, ACC, s);
}

void mul_class::code(ostream &s) {
  // Avalia e1
  this->e1->code(s);
  // Salva e1 na pilha
  emit_push(ACC, s);
  elementsInStack++;

  // Avalia e2
  this->e2->code(s);
  // Copia o objeto resultado de e2 para salvar o resultado
  emit_jal("Object.copy", s);
  // Remove e1 da pilha
  emit_addiu(SP, SP, WORD_SIZE, s);
  elementsInStack--;
  // Salva e1 em T1
  emit_load(T1, 0, SP, s);
  // Salva a cópia de e2 em T2
  emit_move(T2, ACC, s);
  // Carrega os valores inteiros dos objetos
  emit_load(T1, 3, T1, s);
  emit_load(T2, 3, T2, s);
  // Salva a multiplicação em T3
  emit_mul(T3, T1, T2, s);
  // Salva no valor inteiro da cópia de e2 o valor de T3
  // A cópia de e2 está no registrador a0
  emit_store(T3, 3, ACC, s);
}

void divide_class::code(ostream &s) {
  // Avalia e1
  this->e1->code(s);
  // Salva e1 na pilha
  emit_push(ACC, s);
  elementsInStack++;

  // Avalia e2
  this->e2->code(s);
  // Copia o objeto resultado de e2 para salvar o resultado
  emit_jal("Object.copy", s);
  // Remove e1 da pilha
  emit_addiu(SP, SP, WORD_SIZE, s);
  elementsInStack--;
  // Salva e1 em T1
  emit_load(T1, 0, SP, s);
  // Salva a cópia de e2 em T2
  emit_move(T2, ACC, s);
  // Carrega os valores inteiros dos objetos
  emit_load(T1, 3, T1, s);
  emit_load(T2, 3, T2, s);
  // Salva a divisão em T3
  emit_div(T3, T1, T2, s);
  // Salva no valor inteiro da cópia de e2 o valor de T3
  // A cópia de e2 está no registrador a0
  emit_store(T3, 3, ACC, s);
}

void neg_class::code(ostream &s) {
  // Avalia e1
  this->e1->code(s);
  // Copia o objeto do resultado de e1
  emit_jal("Object.copy", s);
  // Carrega o valor inteiro da cópia de e1 em T1
  emit_load(T1, 3, ACC, s);
  // Calcula o valor negativado de T1
  emit_neg(T1, T1, s);
  // Salva o valor inteiro calculado na cópia de e1
  emit_store(T1, 3, ACC, s);
}

void lt_class::code(ostream &s) {
  // Avalia e1
  this->e1->code(s);
  // Salva e1 na pilha
  emit_push(ACC, s);
  elementsInStack++;

  // Avalia e2
  this->e2->code(s);
  // Copia o objeto resultado de e2 para salvar o resultado
  emit_jal("Object.copy", s);
  // Remove e1 da pilha
  emit_addiu(SP, SP, WORD_SIZE, s);
  elementsInStack--;
  // Salva e1 em T1
  emit_load(T1, 0, SP, s);
  // Salva a cópia de e2 em T2
  emit_move(T2, ACC, s);
  // Carrega os valores inteiros dos objetos
  emit_load(T1, 3, T1, s);
  emit_load(T2, 3, T2, s);
  // Salva label para os branches
  int labelLess = labelId++;
  int labelExit = labelId++;
  // Se T1 < T2 vai para labelLess
  emit_blt(T1, T2, labelLess, s);
  // Carrega false em a0
  emit_load_bool(ACC, BoolConst(0), s);
  // Finaliza a operação
  emit_branch(labelExit, s);
  // Se T1 < T2
  emit_label_def(labelLess, s);
  // Carrega true em a0
  emit_load_bool(ACC, BoolConst(1), s);
  // Label de fim da operação
  emit_label_def(labelExit, s);
}

void eq_class::code(ostream &s) {
  // Avalia e1
  this->e1->code(s);
  // Salva e1 na pilha
  emit_push(ACC, s);
  elementsInStack++;

  // Avalia e2
  this->e2->code(s);
  // Remove e1 da pilha
  emit_addiu(SP, SP, WORD_SIZE, s);
  elementsInStack--;
  // Salva e1 em T1
  emit_load(T1, 0, SP, s);
  // Salva e2 em T2
  emit_move(T2, ACC, s);
  // Verifica se e1 e e2 são tipos básicos
  if(e1->type == Bool || e1->type == Str || e1->type == Int) {
    if(e2->type == Bool || e2->type == Str || e2->type == Int) {
      // Se sim, chama a rotina equality_test do cool
      emit_load_bool(ACC, BoolConst(1), s);
      emit_load_bool(A1, BoolConst(0), s);
      emit_jal("equality_test", s);
      return;
    }
  }
  // Se são tipos não básico, verifica se a referência é a mesma
  // Salva label para os branches
  int labelEqual = labelId++;
  int labelExit = labelId++;
  // Se T1 = T2 vai para labelEqual
  emit_beq(T1, T2, labelEqual, s);
  // Carrega false em a0
  emit_load_bool(ACC, BoolConst(0), s);
  // Finaliza a operação
  emit_branch(labelExit, s);
  // Se T1 = T2
  emit_label_def(labelEqual, s);
  // Carrega true em a0
  emit_load_bool(ACC, BoolConst(1), s);
  // Label de fim da operação
  emit_label_def(labelExit, s);
}

void leq_class::code(ostream &s) {
  // Avalia e1
  this->e1->code(s);
  // Salva e1 na pilha
  emit_push(ACC, s);
  elementsInStack++;

  // Avalia e2
  this->e2->code(s);
  // Copia o objeto resultado de e2 para salvar o resultado
  emit_jal("Object.copy", s);
  // Remove e1 da pilha
  emit_addiu(SP, SP, WORD_SIZE, s);
  elementsInStack--;
  // Salva e1 em T1
  emit_load(T1, 0, SP, s);
  // Salva a cópia de e2 em T2
  emit_move(T2, ACC, s);
  // Carrega os valores inteiros dos objetos
  emit_load(T1, 3, T1, s);
  emit_load(T2, 3, T2, s);
  // Salva label para os branches
  int labelLessEqual = labelId++;
  int labelExit = labelId++;
  // Se T1 < T2 vai para labelLessEqual
  emit_bleq(T1, T2, labelLessEqual, s);
  // Carrega false em a0
  emit_load_bool(ACC, BoolConst(0), s);
  // Finaliza a operação
  emit_branch(labelExit, s);
  // Se T1 < T2
  emit_label_def(labelLessEqual, s);
  // Carrega true em a0
  emit_load_bool(ACC, BoolConst(1), s);
  // Label de fim da operação
  emit_label_def(labelExit, s);
}

void comp_class::code(ostream &s) { // Operação not
  // Avalia e1
  this->e1->code(s);
  // Salva em T1 o inteiro do resultado de e1
  emit_load(T1, 3, ACC, s);
  // Salva label para os branches
  int labelZero = labelId++;
  int labelExit = labelId++;
  // Se T1 = 0 vai para labelZero
  emit_beqz(T1, labelZero, s);
  // Carrega false em a0
  emit_load_bool(ACC, BoolConst(0), s);
  // Finaliza a operação
  emit_branch(labelExit, s);
  // Se T1 = 0
  emit_label_def(labelZero, s);
  // Carrega true em a0
  emit_load_bool(ACC, BoolConst(1), s);
  // Label de fim da operação
  emit_label_def(labelExit, s);
}

void int_const_class::code(ostream& s)  
{
  //
  // Need to be sure we have an IntEntry *, not an arbitrary Symbol
  //
  emit_load_int(ACC,inttable.lookup_string(token->get_string()),s);
}

void string_const_class::code(ostream& s)
{
  emit_load_string(ACC,stringtable.lookup_string(token->get_string()),s);
}

void bool_const_class::code(ostream& s)
{
  emit_load_bool(ACC, BoolConst(val), s);
}

void new__class::code(ostream &s) {
  if (this->type_name == SELF_TYPE) {
        // Salva em T1 o endereço da tabela de classes
        emit_load_address(T1, "classPrototypeTable", s);
        // Salva em T2 a tag da classe self
        emit_load(T2, 0, SELF, s);
        // Busca a entrada da tabela que possui o prototipo do objeto
        // Faz T2*2
        emit_add(T2, T2, T2, s);
        // Faz T2*4
        emit_sll(T2, T2, 2, s);
        // Salva em T3 o endereço da linha da tabela de classes
        emit_add(T3, T1, T2, s);
        // Salva em ACC a referência para o prototype da classe
        emit_load(ACC, 0, T3, s);
        // Copia para ACC uma cópia do objeto
        emit_jal("Object.copy", s);
        // Carrega a referência ao init, que é a próxima linha da tabela
        emit_load(T3, 1, T3, s);
        // Executa o init
        emit_jalr(T3, s);
    } else {
      // Salva a label do prototype da classe
      std::string classType = this->type_name->get_string();
      classType += PROTOBJ_SUFFIX;
      // Carrega em a0 o endereço do prototype
      emit_load_address(ACC, classType.c_str(), s);
      // Copia o objeto
      emit_jal("Object.copy", s);
      // Salva a label do init da classe
      classType = type_name->get_string();
      classType += CLASSINIT_SUFFIX;
      // Inicializa o objeto
      emit_jal(classType.c_str(), s);
    }

}

void isvoid_class::code(ostream &s) {
  // Avalia e1
  this->e1->code(s);
  // Salva o resultado de e1 em T1
  emit_move(T1, ACC, s);
  // Salva label para os branches
  int labelZero = labelId++;
  int labelExit = labelId++;
  // Se T1 = 0 vai para labelZero
  emit_beqz(T1, labelZero, s);
  // Carrega false em a0
  emit_load_bool(ACC, BoolConst(0), s);
  // Finaliza a operação
  emit_branch(labelExit, s);
  // Se T1 = 0
  emit_label_def(labelZero, s);
  // Carrega true em a0
  emit_load_bool(ACC, BoolConst(1), s);
  // Label de fim da operação
  emit_label_def(labelExit, s);
}

void no_expr_class::code(ostream &s) {
  // Retorna zero para no_expr
  emit_move(ACC, ZERO, s);
}

void object_class::code(ostream &s) {
  // Verifica se o objeto existe na tabela de símbolos
  if(symbolTable.find(this->name) != symbolTable.end()) {
    int offset = getStackOffset(this->name);
    // Salva o objeto em a0
    emit_store(ACC, offset, SP, s);
    // Tratamento do garbage collector
    if(cgen_Memmgr) {
      emit_addiu(A1, FP, 4*(offset), s);
      emit_jal("_GenGC_Assign", s);
    }
  } else {
    if(name == self) {
      // O objeto é o self, logo salva SELF em a0
      emit_move(ACC, SELF, s);
    } else {
      s << "Erro!" << endl;
    }
  }
}


