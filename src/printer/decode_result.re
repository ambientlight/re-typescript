type type_def =
  | Base(base_type)
  | Optional(type_def)
  | Nullable(type_def)
  | TypeDeclaration((string, string), type_def, list(type_arg))
  | Record(list(type_def))
  | RecordField((string, string), type_def, bool)
  | Union(list(union_member))
  | VariantEnum(list((string, string)))
  | VariantString(list(string))
  | VariantInt(list(int))
  | VariantMixed(list(value_type))
  | Array(type_def)
  | Tuple(list(type_def))
and type_arg = {
  name: string,
  default: option(type_def),
}
and union_member = {
  um_name: string,
  um_type: type_def,
  um_classifier: string,
}
and base_type =
  | String
  | Number
  | Boolean
  | Void
  | Any
  | Arg(string)
  | Ref((string, string), list(type_def))
and value_type =
  | U_String(string)
  | U_Number(int)
  | U_Bool(bool);
