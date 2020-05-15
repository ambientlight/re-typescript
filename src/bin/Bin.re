open Re_typescript_base;

let content = {|
import { Palette } from './createPalette';
import * as React from 'react';
import { CSSProperties } from './withStyles';

type x = string;
type y = number;
type someObj = {
    some: boolean,
    other: string,
    rec: someObj,
};
interface next extends React.CSSProperties<Required<{
  fontFamily: string;
  nested: React.SomeOther<{color: number},string,boolean>;
}>> {
    has: number;
    obj: somObj;
};

type emptyObj = {};
interface emptyI {};

export declare type Subset<T, U> = {
  [key in keyof T]: key extends keyof U ? T[key] : never;
};
|};
let content = {|
interface I_one {
  key_1: string;
  key_2: number;
}
interface I_two extends I_one {
  key_1: number;
  key_3: boolean;
}

type module = number[];
type interfaceA = Array<EmptyI>
|};

let () = {
  let lexbuf = Lexing.from_string(content);

  try(
    Printf.fprintf(
      stdout,
      "%s",
      Re_typescript_printer.print_from_ts(
        ~ctx=Re_typescript_printer.Config.defaultConfig,
        Parser.main(Lexer.read, lexbuf),
      ),
    )
  ) {
  | Lexer.SyntaxError(msg) => Printf.fprintf(stderr, "%s", msg)
  | Parser.Error =>
    Printf.fprintf(
      stderr,
      "At offset %d: syntax error.\n%s",
      Lexing.lexeme_start(lexbuf),
      Lexing.(
        try(
          "\n\n\""
          ++ sub_lexeme(
               lexbuf,
               lexeme_start(lexbuf) - 15,
               lexeme_end(lexbuf),
             )
          ++ "\"\n\n"
        ) {
        | _ => lexeme(lexbuf)
        }
      ),
    )
  | e => raise(e)
  };
};
