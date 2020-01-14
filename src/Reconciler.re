

let rec unmount = (node: Types.Node.t, parent: Types.Node.t, index: int): unit => {
  if (!Node.getUnmountSchedule(node)) {
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

      nodes |> Array.iteri((i, child) => {
        unmount(child, node, i);
      });

      Node.clearNode(parent, index);
      Node.unmount(node);
    });
  }
};

let rec renderResult = (nodes: Types.Renderer.Result.t, parent: Types.Node.t, index: int, root: Types.Node.t) => {
  /**
   * Since there are dependency cycles, load the render function
   */
  let render: Types.Reconciler.Render.t = ModuleResolver.getModule("render");
  switch (nodes) {
    | Fragment(list) => {
      list |> List.iteri((id, node) => {
        renderResult(node, parent, id, root);
      });
    }
    | Some(node) => {
      render(node, parent, Some(index), Some(root));
    }
    | None => {
      Node.forEachNode(parent, (i, child) => {
        unmount(child, parent, i);
      });
    };
  };
};

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

          renderResult(childNode, node, 0, root);
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