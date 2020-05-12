%token TYPE INTERFACE
%token <string> IDENT
%token <string> STRING
%token EQUALS
%token EXTENDS
%token GT LT
%token PRIM_STRING PRIM_NUMBER PRIM_BOOLEAN
%token SEMICOLON COLON COMMA QMARK DOT
%token LCURLY RCURLY LPAREN RPAREN
%token IMPORT FROM STAR AS
%token EOF

%start <Ts.toplevel> main
%%

let main :=
  | imports = import*; types = type_def*; EOF; { { types; imports } }

type_def:
  | TYPE; name = IDENT; EQUALS; t = type_; SEMICOLON? { `TypeDef(name, t) }
  | INTERFACE; name = IDENT; extends = extends; LCURLY; obj = separated_or_terminated_list(SEMICOLON, obj_field); RCURLY; SEMICOLON? { `InterfaceDef(name, extends, obj) }

let extends :=
  | { [] }
  | EXTENDS; v = ref_; { v }

let ref_ :=
  ~ = separated_or_terminated_list(DOT, ref_part); <>

let ref_part :=
  | name = IDENT; { (name, []) }
  | name = IDENT; LT; t = separated_or_terminated_list(COMMA, type_); GT; { (name, t) }


separated_or_terminated_list(separator, X):
  | x=X { [x] }
  | x=X separator { [x] }
  | x=X separator xs=separated_or_terminated_list(separator, X) { x :: xs }

obj_field:
  | key = STRING; r = QMARK?; COLON; t = type_    { {Ts.key; type_ = t; required = match r with | None -> false | Some _ -> true } }
  | key = IDENT; r = QMARK?; COLON; t = type_     { {Ts.key; type_ = t; required = match r with | None -> false | Some _ -> true } } ;

type_:
  | PRIM_STRING       { `String }
  | PRIM_NUMBER       { `Number }
  | PRIM_BOOLEAN      { `Boolean }
  | LCURLY; obj = separated_or_terminated_list(COMMA, obj_field); RCURLY; { `Obj(obj) }
  | r = ref_          { `Ref(r) };




let import :=
  | IMPORT; name = import_alias; FROM; path = STRING; SEMICOLON?; { { path; name } }

let import_alias :=
  | name = IDENT; { `Named(name) }
  | name = IDENT; AS; alias = IDENT; { `Alias(`Named(name), alias) }
  | STAR; AS; alias = IDENT; { `Alias(`Star, alias) }
  | LCURLY; lst = separated_or_terminated_list(COMMA, import_alias); AS; alias = IDENT; RCURLY; { `Alias(`List(lst), alias) }
  | LCURLY; lst = separated_or_terminated_list(COMMA, import_alias); RCURLY; { `List(lst) }