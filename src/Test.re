[@bs.val]
external setTimeout: ('input => unit, int) => unit = "setTimeout";

/**
 * Internal component that re-renders on mount.
 */
module Test {
  let make = Component.make("Test", () => {
    let (a, setA): (int, State.dispatch(int)) = State.use(100);
    let (b, setB): (int, State.dispatch(int)) = State.use(200);

    /**
     * Although both dispatches can trigger the re-render cycle
     * All simultaneous requests would be merged.
     */
    Effect.use(() => {
      setA(_ => 1000);
      setB(_ => 2000);

      Some(() => {
        Js.log("unmounted");
      });
    }, []);

    Js.log(a);
    Js.log(b);
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
    let (state, setState) = State.use(false);

    Effect.use(() => {
      setTimeout(() => {
        setState(_ => !state);
      }, 250);
      None;
    }, [state]);

    Js.log(state);

    if (!state) {
      Fragment.make({
        children: children,
      });
    } else {
      Fragment.make({
        children: [],
      });
    };
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
  }, 1000);
}, 1000);
