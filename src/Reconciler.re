

let rec unmount = (node: Types.Node.t, parent: Types.Node.t, index: int): unit => {
  if (!Node.isUnmounted(node) && !Node.getUnmountSchedule(node)) {
    Node.setUnmountSchedule(node, true);

    Utils.scheduleAsync(() => {
      Node.setUnmountSchedule(node, false);

      /**
       * Run all effect cleanup
       */
      Node.forEachState(node, (_, state) => {
        switch (state) {
          | Some(Types.State.EffectCleanup(Some(cleanup))) => {
            cleanup();
          }
          | _ => ();
        }
      });

      let nodes = Node.nodes(node);

      nodes |> List.iteri((i, child) => {
        unmount(child, node, i);
      });

      Node.clearNode(parent, index);
      Node.unmount(node);
    });
  }
};

let request = (node: Types.Node.t, parent: Types.Node.t, index: int, root: Types.Node.t): unit => {
  if (!Node.isUnmounted(node) && !Node.getMountSchedule(node)) {
    Node.setMountSchedule(node, true);

    Utils.scheduleAsync(() => {
      if (!Node.isUnmounted(node)) {
        Node.setMountSchedule(node, false);
        /**
         * Set render context
         */
        RenderContext.setContext({
          node,
          parent,
          root,
          index,
          slot: ref(0),
        });

        /**
         * Set parent node for node
         */
        Node.mount(node, parent);

        /**
         * Perform render
         */
        let component = Node.component(node)
        
        switch(component) {
          | Some(component) => {
            let childNode = component.render(Node.props(node));
            
            /**
             * Since there are dependency cycles, load the render function
             */
            let render: Types.Reconciler.Render.t = ModuleResolver.getModule("render");
            switch (childNode) {
              | Some(actual) => render(actual, node, Some(0), Some(root));
              | None => {
                switch (Node.getNode(node, 0)) {
                  | Some(actual) => unmount(actual, node, 0);
                  | None => ();
                }
              };
            }
          }
          | None => ();
        }

        /**
         * Reset render context
         */
        RenderContext.popContext();

        /**
         * Render complete, add node to parent
         */
        Node.setNode(parent, index, node);

        /**
         * Run all effects
         */
        Node.forEachState(node, (id, state) => {
          switch (state) {
            | Some(Types.State.Effect(effect)) => {
              let cleanup = effect();

              Node.setState(node, id, EffectCleanup(cleanup));
            }
            | _ => ();
          }
        });
      }
    });
  }
};

let render: Types.Reconciler.Render.t = (node, parent, index, root): unit => {
  let actualRoot = switch (root) {
    | Some(rn) => rn;
    | None => parent;
  };

  let actualIndex = switch (index) {
    | Some(value) => value;
    | None => 0;
  };

  /**
   * Get the equivalent node slot
   */
  let renderedNode = Node.getNode(parent, actualIndex);

  switch (renderedNode) {
    | Some(actualNode) => {
      /**
       * Compare both component, else props
       */
      if (Node.component(actualNode) != Node.component(node)) {
        /**
         * Unmount rendered node
         */
        unmount(actualNode, parent, actualIndex);

        /**
         * Request an initial render
         */
        request(node, parent, actualIndex, actualRoot);
      } else if (Node.toEquatable(actualNode) != Node.toEquatable(node)) {
        /**
         * Pass nodes and states
         */
        Node.replicate(node, actualNode);

        /**
         * Request a re-render
         */
        request(node, parent, actualIndex, actualRoot);
      }
    }
    | None => {
      request(node, parent, actualIndex, actualRoot);
    }
  }
}

ModuleResolver.setModule("render", render);