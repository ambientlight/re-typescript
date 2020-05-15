open TestFramework;
open Re_typescript_base;

let config = Re_typescript_printer.Config.defaultConfig;
let print = (~ctx=config, value) =>
  Re_typescript_printer.print_from_ts(
    ~ctx,
    Parser.main(Lexer.read, Lexing.from_string(value)),
  );

/********************************************
  Basic type syntax
 ********************************************/
describe("basic syntax for types", ({test, _}) => {
  test("basic type definition", ({expect, _}) => {
    expect.string(print("type t_string = string;")).toMatchSnapshot();
    expect.string(print("type t_number = number;")).toMatchSnapshot();
    expect.string(print("type t_bool = boolean;")).toMatchSnapshot();
  });

  test("basic type definition without semicolon", ({expect, _}) => {
    expect.string(print("type t_string = string")).toMatchSnapshot()
  });
});

/********************************************
  Type names
 ********************************************/
describe("generates valid type names", ({test, _}) => {
  test("capitalized", ({expect, _}) => {
    expect.string(print("type T_string = string")).toMatchSnapshot()
  });
  test("reserved keyword", ({expect, _}) => {
    expect.string(print("type rec = string")).toMatchSnapshot();
    expect.string(print("type sig = string")).toMatchSnapshot();
  });
  test("invalid characters", ({expect, _}) => {
    expect.string(print("type st$ring = string")).toMatchSnapshot();
    expect.string(print("type t_$string = string")).toMatchSnapshot();
    expect.string(print("type $_string = string")).toMatchSnapshot();
    expect.string(print("type t_$string = string")).toMatchSnapshot();
    // Invalid TS typenames should not be parsed at all
    expect.fn(() => print("type 1_string = string")).toThrowException(
      Parser.Error,
    );
    expect.fn(() => print("type st.ring = string")).toThrowException(
      Parser.Error,
    );
  });
});

/********************************************
  Config Flags
 ********************************************/
describe("config flags effects", ({test, _}) => {
  test("number_mode default value", ({expect, _}) => {
    let ts = "type t_number = number";
    expect.string(print(ts)).toMatch(
      print(~ctx={...config, number_mode: Float}, ts),
    );
  });

  test("number_mode", ({expect, _}) => {
    let ts = "type t_number = number";
    expect.string(print(~ctx={...config, number_mode: Float}, ts)).
      toMatchSnapshot();
    expect.string(print(~ctx={...config, number_mode: Int}, ts)).
      toMatchSnapshot();
    // Unboxed not implemented yet
    expect.fn(() => print(~ctx={...config, number_mode: Unboxed}, ts)).
      toThrow();
  });
});

/********************************************
  Interfaces
 ********************************************/
describe("interfaces", ({test, _}) => {
  test("object & interface syntax create equal types", ({expect, _}) => {
    expect.string(print({|type Eqiv = { field: string }|})).toEqual(
      print({|
      interface Eqiv {
        field: string;
      }
    |}),
    )
  })
});

describe("interface extension", ({test, _}) => {
  test("adds fields from other interfaces", ({expect, _}) => {
    expect.string(
      print(
        {|
      interface I_a {
        field_1: string;
        field_2: string;
      }
      interface I_b extends I_a {
        field_3: number
      }
    |},
      ),
    ).
      toMatchSnapshot()
  });
  test("fields from current interface take priority", ({expect, _}) => {
    expect.string(
      print(
        {|
      interface I_a {
        field_1: string;
        field_2: string;
      }
      interface I_b extends I_a {
        field_1: boolean;
        field_3: number
      }
    |},
      ),
    ).
      toMatchSnapshot()
  });
});

/********************************************
  Reference resolution
 ********************************************/
describe("reference resolution", ({test, _}) => {
  test("resolve simple definitions", ({expect, _}) => {
    expect.string(
      print({|
      type t_str = string;
      type t_ref = t_str;
    |}),
    ).
      toMatchSnapshot()
  });

  test("resolve in interface fields", ({expect, _}) => {
    expect.string(
      print(
        {|
      type t_str = string;
      interface I_Ref {
        f_num: number;
        f_str: t_str;
      };
      type r_ref = {
        inner: I_Ref
      }
    |},
      ),
    ).
      toMatchSnapshot()
  });

  test("resolve recursively", ({expect, _}) => {
    expect.string(
      print(
        {|
      interface I_Ref {
        f_num: number;
        f_inner: I_Ref
      };
    |},
      ),
    ).
      toMatchSnapshot()
  });
});
