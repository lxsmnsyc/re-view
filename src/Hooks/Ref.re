type t('a) = ref('a);

let use = (initialValue: 'a): ref('a) => {
  if (RenderContext.isActive()) {
    let { node, slot }: Types.RenderContext.t = RenderContext.getContext();

    let current = slot^;
    let state: option(ref('a)) = Node.getState(node, current);

    switch (state) {
      | Some(value) => {
        slot := current + 1
        value;
      }
      | None => {
        let value = ref(initialValue);
        Node.setState(node, current, value);
        slot := current + 1;
        value;
      }
    }
  } else {
    raise(Exception.IllegalRenderContextAccess);
  }
};