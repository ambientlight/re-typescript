type toplevel = {
  types: list((type_def, bool)),
  imports: list(import),
}
and type_def = [
  | `TypeDef(string, type_)
  | `InterfaceDef(string, ref_, list(obj_field))
  | `EnumDef(string, list(enum_field), bool)
]
and type_ = [
  | `String
  | `Number
  | `Boolean
  | `Null
  | `Undefined
  | `Void
  | `Any
  | `Union(list(union_member))
  | `Array(type_)
  | `Tuple(list(type_))
  | `Obj(list(obj_field))
  | `Ref(ref_)
  | `TypeExtract(ref_, list(string))
]
and union_member = [ | `U_String(string) | `U_Number(int) | `U_Type(type_)]
and prim_value = [ | `V_String(string) | `V_Number(int)]
and enum_field = {
  key: string,
  default: option(prim_value),
}
and obj_field = {
  key: string,
  type_,
  optional: bool,
  readonly: bool,
}
and ref_ = list((string, list(type_)))
and import = {
  path: string,
  name: importName,
}
and importName = [
  | `Named(string)
  | `Alias(importName, string)
  | `Star
  | `List(list(importName))
];

type extract_wrapper = [ | `T(type_def, bool) | `I(import) | `Empty];
let make_top_level = (lst: list(extract_wrapper)) =>
  lst
  |> Tablecloth.List.fold_left(
       ~f=
         (value, p) =>
           switch (value) {
           | `I(import) => {
               ...p,
               imports: Tablecloth.List.append(p.imports, [import]),
             }
           | `T(type_def) => {
               ...p,
               types: Tablecloth.List.append(p.types, [type_def]),
             }
           | `Empty => p
           },
       ~initial={imports: [], types: []},
     );
