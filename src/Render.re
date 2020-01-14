let call = (node: option(Types.Node.t), root: Types.Node.t) => {
  switch (node) {
    | Some(actual) => Reconciler.render(actual, root, None, None);
    | None => ();
  }
};