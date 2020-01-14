
let use = (effect: Types.State.Effect.t, dependency: 'dependency) => {
  if (RenderContext.isActive()) {
    let { node, slot }: Types.RenderContext.t = RenderContext.getContext();
    /**
     * Get state slots
     */
    let current = slot^;
    let effectSlot = Node.getState(node, current);
    let dependencySlot = Node.getState(node, current + 1);

    let shouldUpdate = switch (dependencySlot) {
      | Some(Dependency(actualDeps)) => actualDeps != dependency;
      | Some(_) => raise(Exception.IllegalSlotAccess);
      | None => true;
    };

    let cleanup = switch (effectSlot) {
      | Some(EffectCleanup(clean)) => clean;
      | Some(Effect(_)) => Some(() => ());
      | Some(_) => raise(Exception.IllegalSlotAccess);
      | None => Some(() => ());
    }

    if (shouldUpdate) {
      switch (cleanup) {
        | Some(clean) => clean();
        | None => ();
      }
      Node.setState(node, current, Effect(effect));
      Node.setState(node, current + 1, Dependency(dependency));
    }

    slot := current + 2;
  }
};