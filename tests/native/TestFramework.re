include Rely.Make({
  let config =
    Rely.TestFrameworkConfig.initialize({
      snapshotDir: "tests/native/__snapshots__",
      projectDir: "src",
    });
});

open Re_typescript_base;

let config = Re_typescript_printer.Config.defaultConfig;
let print = (~ctx=config, value) =>
  Re_typescript_printer.print_from_ts(
    ~ctx,
    Parser.main(Lexer.read, Lexing.from_string(value)),
  );
