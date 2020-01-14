type dispatch('state) = ('state => 'state) => unit;
type tuple('state) = ('state, dispatch('state));

let use = (initialState: 'state): tuple('state) => {
  if (RenderContext.isActive()) {
    let { node, root, parent, index, slot }: Types.RenderContext.t = RenderContext.getContext();

    /**
     * Get state slots
     */
    let current = slot^;
    let state = Node.getState(node, current);
    let dispatch = Node.getState(node, current + 1);

    let actualState: 'state = {
      switch(state) {
        | Some(BasicState(value)) => value;
        | Some(_) => raise(Exception.IllegalSlotAccess);
        | None => {
          /**
           * Set slot values
           */
          Node.setState(node, current, BasicState(initialState));

          initialState;
        }
      }
    };

    let actualDispatch = {
      switch (dispatch) {
        | Some(SetState(value)) => value;
        | Some(_) => raise(Exception.IllegalSlotAccess);
        | None => {
          /**
           * Create new dispatch
           */
          let newDispatch = (newState: 'state => 'state) => {
            let prevState = Node.getState(node, current);

            switch (prevState) {
              | Some(BasicState(value)) => {
                let actual = newState(value);
                if (value != actual) {
                  Node.setState(node, current, BasicState(actual));
                  Reconciler.request(node, parent, index, root);
                }
              }
              | Some(_) => raise(Exception.IllegalSlotAccess);
              | None => raise(Exception.IllegalSlotAccess);
            }
          };
          Node.setState(node, current + 1, SetState(newDispatch));

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