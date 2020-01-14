type props = {
  children: list(option(Types.Node.t)),
};

let make = Component.make("Fragment", ({ children }: props) => {
  let { node, root }: Types.RenderContext.t = RenderContext.getContext();

  children |> List.iteri((id, child) => {
    switch (child) {
      | Some(actual) => {
        Reconciler.render(actual, node, Some(id), Some(root));
      }
      | None => {
        switch (Node.getNode(root, id)) {
          | Some(actual) => Reconciler.unmount(actual, root, id);
          | None => ();
        }
      }
    }
  });

  None;
});