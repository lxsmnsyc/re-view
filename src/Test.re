[@bs.val]
external setTimeout: ('input => unit) => unit = "setTimeout";

/**
 * Internal component that re-renders on mount.
 */
module Test {
  let make = Component.make("Test", () => {
    let (a, setA): (int, State.dispatch(int)) = State.use(100);
    let (b, setB): (int, State.dispatch(int)) = State.use(200);

    Js.log(a);
    Js.log(b);

    /**
     * Although both dispatches can trigger the re-render cycle
     * All simultaneous requests would be merged.
     */
    setA(_ => 1000);
    setB(_ => 2000);

    None;
  });
};

/**
 * Main node
 */
module App {
  type props = {
    children: list(option(Types.Node.t)),
  };

  let make = Component.make("App", ({ children }: props) => {
    /**
     * Does not re-render again don't worry ;)
     */
    Js.log("rendered again :/");

    Fragment.make({
      children: children,
    });
  });
};

/**
 * Root node
 */
let root = Node.make(None, None);

/**
 * Render the top node to the root
 */
Render.call(App.make({
  children: [
    Test.make(),
    Test.make(),
  ],
}), root);

let dir: 'a => unit = [%bs.raw {|
  function dir(object) {
    console.dir(object, { depth: null });
  }
|}];

/**
 * Run a timeout process
 */
setTimeout(() => {
  /**
   * Check root structure
   */
  dir(root);

  /**
   * Try to re-render the app
   * Take note of the same structure
   */
  Render.call(App.make({
    children: [
      Test.make(),
      Test.make(),
    ],
  }), root);

  /**
   * Re-check again
   */
  setTimeout(() => {
    dir(root);
  });
});
