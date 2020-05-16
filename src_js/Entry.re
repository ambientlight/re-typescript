let content = {|
import { Palette } from './createPalette';
import * as React from 'react';
import { CSSProperties } from './withStyles';

interface I_one {
  key_1: string;
  key_2: number;
}
interface I_two extends I_one {
  key_1: number;
  key_3: boolean;
}

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

type module = number[];
type interfaceA = Array<EmptyI>

interface I_a {
  field_1: string;
  field_2: string;
  field_3: {
    inline: boolean;
    nested: string;
  }
}
|};

module Highlight = {
  [@react.component] [@bs.module "react-highlight.js"]
  external make: (~language: string, ~children: string) => React.element =
    "default";
};

module CodeMirror = {
  [%bs.raw "require('codemirror/lib/codemirror.css')"];
  [%bs.raw "require('codemirror/mode/javascript/javascript')"];
  type ts_mode ={
    name: string,
    typescript:bool,
  };
  type options = {
    lineNumbers: bool,
    mode: ts_mode,
    theme: string,
  };
  [@react.component] [@bs.module]
  external make:
    (
      ~value: string,
      ~onChange: string => unit,
      ~options: option(options)=?,
      ~className: option(string)=?,
      ~preserveScrollPosition: option(bool)=?
    ) =>
    React.element =
    "react-codemirror";
};

module X = {
  [@react.component]
  let make = () => {
    let (v, setV) = React.useReducer((_, v) => v, content);
    let (re, setRe) = React.useReducer((_, v) => v, true);

    <>
      <CodeMirror
        className="editor"
        value=v
        onChange=setV
        preserveScrollPosition=true
        options={lineNumbers: true, mode: {name: "javascript", typescript: true }, theme:"monokai" }
      />
      {try(
         <div className="display">
           <button className="switch-button" onClick={_ => setRe(!re)}>
             (re ? "ReasonML" : "ocaml")->React.string
           </button>
           <pre>
             <Highlight language={re ? "reasonml" : "ocaml"}>
               {BsPrinter.print(~re, v)}
             </Highlight>
           </pre>
         </div>
       ) {
       | BsPrinter.ParseError(e) =>
         <div
           className="error"
           style={ReactDOMRe.Style.make(~padding="15px", ())}
           dangerouslySetInnerHTML={"__html": e}
         />
       }}
    </>;
  };
};

ReactDOMRe.renderToElementWithId(<X />, "app");
