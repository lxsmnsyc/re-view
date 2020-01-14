
let request = (node: Types.Node.t, parent: Types.Node.t, index: int, root: Types.Node.t): unit => {
  if (!Node.getMountSchedule(node)) {
    Node.setMountSchedule(node, true);

    Utils.scheduleAsync(() => {
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

          switch (childNode) {
            | Some(actual) => {
              /**
               * Since there are dependency cycles, load the render function
               */
              let render: Types.Reconciler.Render.t = ModuleResolver.getModule("render");

              /**
               * Render child node to node
               */
              render(actual, node, None, Some(root));
            }
            | None => ();
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
    });
  }
};

let rec unmount = (node: Types.Node.t, parent: Types.Node.t, index: int): unit => {
  if (!Node.getUnmountSchedule(node)) {
    Node.setUnmountSchedule(node, true);

    Utils.scheduleAsync(() => {
      Node.setUnmountSchedule(node, false);
      let nodes = Node.nodes(node);

      nodes |> Array.iteri((i, child) => {
        unmount(child, node, i);
      });

      Node.clearNode(parent, index);
      Node.unmount(node);
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