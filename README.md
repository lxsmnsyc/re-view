# re-view
Build user interfaces in ReasonML

## Install

## Features

- Tree-diffing using the built-in equality comparison.
  - We don't have to implement our own fancy diffing process for our props/states, BuckleScript does all the job for us.

- Natural children node diffing.
  - Treats Nodes as immutable data, thus creating components that requires multiple children is covered
  by the diffing process.

- Simple reconciliation process written in ReasonML.

- Hooks


## Example

### Reason

```reason
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
```

### Output


```bash
rendered again :/
100
100
1000
1000
<ref *2> Node {
  props: undefined,
  component: undefined,
  state: [],
  nodes: [
    <ref *1> Node {
      props: {
        children: [
          Node {
            props: 0,
            component: { name: 'Test', render: [Function (anonymous)] },
            state: [ 1000, [Function: newDispatch] ],
            nodes: [],
            mountSchedule: false,
            parent: [Circular *1]
          },
          [
            Node {
              props: 0,
              component: { name: 'Test', render: [Function (anonymous)] },
              state: [ 1000, [Function: newDispatch] ],
              nodes: [],
              mountSchedule: false,
              parent: [Circular *1]
            },
            0
          ]
        ]
      },
      component: { name: 'App', render: [Function (anonymous)] },
      state: [],
      nodes: [
        Node {
          props: 0,
          component: { name: 'Test', render: [Function (anonymous)] },
          state: [ 1000, [Function: newDispatch] ],
          nodes: [],
          mountSchedule: false,
          parent: [Circular *1]
        },
        Node {
          props: 0,
          component: { name: 'Test', render: [Function (anonymous)] },
          state: [ 1000, [Function: newDispatch] ],
          nodes: [],
          mountSchedule: false,
          parent: [Circular *1]
        }
      ],
      mountSchedule: false,
      parent: [Circular *2]
    }
  ]
}
<ref *2> Node {
  props: undefined,
  component: undefined,
  state: [],
  nodes: [
    <ref *1> Node {
      props: {
        children: [
          Node {
            props: 0,
            component: { name: 'Test', render: [Function (anonymous)] },
            state: [ 1000, [Function: newDispatch] ],
            nodes: [],
            mountSchedule: false,
            parent: [Circular *1]
          },
          [
            Node {
              props: 0,
              component: { name: 'Test', render: [Function (anonymous)] },
              state: [ 1000, [Function: newDispatch] ],
              nodes: [],
              mountSchedule: false,
              parent: [Circular *1]
            },
            0
          ]
        ]
      },
      component: { name: 'App', render: [Function (anonymous)] },
      state: [],
      nodes: [
        Node {
          props: 0,
          component: { name: 'Test', render: [Function (anonymous)] },
          state: [ 1000, [Function: newDispatch] ],
          nodes: [],
          mountSchedule: false,
          parent: [Circular *1]
        },
        Node {
          props: 0,
          component: { name: 'Test', render: [Function (anonymous)] },
          state: [ 1000, [Function: newDispatch] ],
          nodes: [],
          mountSchedule: false,
          parent: [Circular *1]
        }
      ],
      mountSchedule: false,
      parent: [Circular *2]
    }
  ]
}
```