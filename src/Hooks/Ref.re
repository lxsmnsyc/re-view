
let use = (initialValue: 'state): ref('state) => {
  if (RenderContext.isActive()) {
    let { node, slot }: Types.RenderContext.t = RenderContext.getContext();

    let current = slot^;
    let state = Node.getState(node, current);

    switch (state) {
      | Some(Ref(value)) => {
        slot := current + 1
        value;
      }
      | Some(_) => raise(Exception.IllegalSlotAccess);
      | None => {
        let value = ref(initialValue);
        Node.setState(node, current, Ref(value));
        slot := current + 1;
        value;
      }
    }
  } else {
    raise(Exception.IllegalRenderContextAccess);
  }
};