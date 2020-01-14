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
        | Some(ReducerState(value)) => value;
        | Some(_) => raise(Exception.IllegalSlotAccess);
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
        | Some(ReducerDispatch(value)) => value;
        | Some(_) => raise(Exception.IllegalSlotAccess);
        | None => {
          /**
           * Create new dispatch
           */
          let newDispatch = (action: 'action) => {
            let prevState = Node.getState(node, current);

            switch (prevState) {
              | Some(ReducerState(value)) => {
                let newState = reducer(value, action);
                if (value != newState) {
                  Node.setState(node, current, ReducerState(newState));
                  Reconciler.request(node, parent, index, root);
                }
              }
              | Some(_) => raise(Exception.IllegalSlotAccess);
              | None => raise(Exception.IllegalSlotAccess);
            }
          };
          Node.setState(node, current + 1, ReducerDispatch(newDispatch));

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