type props = {
  children: list(option(Types.Node.t)),
}

let make = Component.make("Fragment", ({ children }: props) => {
  let { parent, root }: Types.RenderContext.t = RenderContext.getContext();

  children |> List.iteri((int, i) => {
    switch (i) {
      | Some(node) => Reconciler.render(node, parent, Some(int), Some(root));
      | None => ();
    };
  });

  None;
});
