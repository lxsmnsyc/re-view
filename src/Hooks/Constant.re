type t('a) = ref('a);

let use = (supplier: unit => 'a): 'a => {
  if (RenderContext.isActive()) {
    let { node, slot }: Types.RenderContext.t = RenderContext.getContext();

    let current = slot^;
    let state: option('a) = Node.getState(node, current);

    switch (state) {
      | Some(value) => {
        slot := current + 1
        value;
      }
      | None => {
        let value = supplier();
        Node.setState(node, current, value);
        slot := current + 1;
        value;
      }
    }
  } else {
    raise(Exception.IllegalRenderContextAccess);
  }
};