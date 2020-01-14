type t('a) = ref('a);

let use = (supplier: unit => 'state): 'state => {
  if (RenderContext.isActive()) {
    let { node, slot }: Types.RenderContext.t = RenderContext.getContext();

    let current = slot^;
    let state = Node.getState(node, current);

    switch (state) {
      | Some(Constant(value)) => {
        slot := current + 1
        value;
      }
      | Some(_) => raise(Exception.IllegalSlotAccess);
      | None => {
        let value = supplier();
        Node.setState(node, current, Constant(value));
        slot := current + 1;
        value;
      }
    }
  } else {
    raise(Exception.IllegalRenderContextAccess);
  }
};