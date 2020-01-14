let call = (node: Types.Renderer.Result.t, root: Types.Node.t) => {
  Reconciler.renderResult(node, root, 0, root);
};
