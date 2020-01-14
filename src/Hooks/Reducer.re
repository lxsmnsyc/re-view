type reducer('state, 'action) = ('state, 'action) => 'state;
type dispatch('action) = 'action => unit;

let use = (reducer: reducer('state, 'action), initialState: 'state): ('state, dispatch('action)) => {
  if (RenderContext.isActive()) {
    let { node, root, parent, index, slot }: Types.RenderContext.t = RenderContext.getContext();

    let current = slot^;
    let state = Node.getState(node, current);
    let dispatch = Node.getState(node, current + 1);

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

    let actualDispatch: dispatch('action) = {
      switch (dispatch) {
        | Some(value) => value;
        | None => {
          /**
           * Create new dispatch
           */
          let newDispatch = (action: 'action) => {
            let prevState: option('state) = Node.getState(node, current);

            switch (prevState) {
              | Some(value) => {
                let newState = reducer(value, action);
                if (value != newState) {
                  Node.setState(node, current, newState);
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