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

module X = {
  [@react.component]
  let make = () => {
    let (v, setV) = React.useReducer((_, v) => v, content);

    <>
      <textarea
        value=v
        onChange={e => setV(e->ReactEvent.Form.target##value)}
      />
      <div className="display">
        <pre dangerouslySetInnerHTML={"__html": BsPrinter.print(v)} />
      </div>
    </>;
  };
};

ReactDOMRe.renderToElementWithId(<X />, "app");