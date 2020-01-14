[@bs.val]
external setTimeout: ('input => unit) => unit = "setTimeout";

/**
 * Internal component that re-renders on mount.
 */
module Test {
  let make = Component.make("Test", () => {
    let (state, setState): (int, State.dispatch(int)) = State.use(100);

    Js.log(state);

    setState(_ => 1000);

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

    Fragment.make(children)
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
