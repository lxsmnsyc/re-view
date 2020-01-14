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
200
100
200
1000
2000
1000
2000
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
            state: [
              1000,
              [Function: newDispatch],
              2000,
              [Function: newDispatch]
            ],
            nodes: [],
            mountSchedule: false,
            parent: [Circular *1]
          },
          [
            Node {
              props: 0,
              component: { name: 'Test', render: [Function (anonymous)] },
              state: [
                1000,
                [Function: newDispatch],
                2000,
                [Function: newDispatch]
              ],
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
          state: [
            1000,
            [Function: newDispatch],
            2000,
            [Function: newDispatch]
          ],
          nodes: [],
          mountSchedule: false,
          parent: [Circular *1]
        },
        Node {
          props: 0,
          component: { name: 'Test', render: [Function (anonymous)] },
          state: [
            1000,
            [Function: newDispatch],
            2000,
            [Function: newDispatch]
          ],
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
            state: [
              1000,
              [Function: newDispatch],
              2000,
              [Function: newDispatch]
            ],
            nodes: [],
            mountSchedule: false,
            parent: [Circular *1]
          },
          [
            Node {
              props: 0,
              component: { name: 'Test', render: [Function (anonymous)] },
              state: [
                1000,
                [Function: newDispatch],
                2000,
                [Function: newDispatch]
              ],
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
          state: [
            1000,
            [Function: newDispatch],
            2000,
            [Function: newDispatch]
          ],
          nodes: [],
          mountSchedule: false,
          parent: [Circular *1]
        },
        Node {
          props: 0,
          component: { name: 'Test', render: [Function (anonymous)] },
          state: [
            1000,
            [Function: newDispatch],
            2000,
            [Function: newDispatch]
          ],
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

## How does it work?

### Virtual DOM and Reconciliation

Unlike most virtual DOMs which rebuilds the tree from ground-up, compares states, props, etc., ReView is a little different.

What ReView does is it only performs the rebuilding process on the specific parts of the tree that requested it, and so it knows when to update the nodes by in-place diffing. The nodes requests whenever they wanted to be rendered again, and if they do, the diffing process then occurs on that node, and then performs the nested diffing if and only if the new node that is proposed to replace the old node has different children nodes.
