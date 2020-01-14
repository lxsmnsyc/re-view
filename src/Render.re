let call = (node: option(Types.Node.t), root: Types.Node.t) => {
  switch (node) {
    | Some(actual) => Reconciler.render(actual, root, Some(0), Some(root));
    | None => {
      switch (Node.getNode(root, 0)) {
        | Some(actual) => Reconciler.unmount(actual, root, 0);
        | None => ();
      }
    }
  }
};
