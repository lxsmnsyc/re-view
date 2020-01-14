type dispatch('state) = ('state => 'state) => unit;
type tuple('state) = ('state, dispatch('state));

let use = (initialState: 'state): tuple('state) => {
  if (RenderContext.isActive()) {
    let { node, root, parent, index, slot }: Types.RenderContext.t = RenderContext.getContext();

    /**
     * Get state slots
     */
    let current = slot^;
    let state: option('state) = Node.getState(node, current);
    let dispatch: option(dispatch('state)) = Node.getState(node, current + 1);

    let actualState: 'state = {
      switch(state) {
        | Some(value) => value;
        | None => {
          /**
           * Set slot values
           */
          Node.setState(node, current, initialState);

          initialState;
        }
      }
    };

    let actualDispatch = {
      switch (dispatch) {
        | Some(value) => value;
        | None => {
          /**
           * Create new dispatch
           */
          let newDispatch = (newState: 'state => 'state) => {
            let prevState: option('state) = Node.getState(node, current);

            switch (prevState) {
              | Some(value) => {
                let actual = newState(value);
                if (value != actual) {
                  Node.setState(node, current, actual);
                  Reconciler.request(node, parent, index, root);
                }
              }
              | None => raise(Exception.IllegalSlotAccess);
            }
          };
          Node.setState(node, current + 1, newDispatch);

          newDispatch;
        }
      }
    };
   

    /**
     * Update slot
     */
    slot := current + 2;

    (actualState, actualDispatch);
  } else {
    raise(Exception.IllegalRenderContextAccess);
  }
};