open Reconciler;
open Let;

module Engine = (Reconciler: ReconcilerType) => {
  module Fiber = {
    type t = {
      constructor: ref(option(Opaque.t)),
      /**
       * Fiber tag
       */
      fiberTag: Tags.Fiber.t,
      /**
       * Work tag
       */
      workTag: ref(Tags.Work.t),
      /**
       * Pointers
       */
      parent: ref(option(t)),
      alternate: ref(option(t)),
      sibling: ref(option(t)),
      child: ref(option(t)),
      children: ref(option(Element.t)),
      /**
       * Mapping
       */
      key: ref(option(string)),
      index: ref(int),
      indeces: ref(int),
      map: Opaque.Map.t,
      /**
       * Reference
       */
      ref: ref(option(Reference.t(Opaque.t))),
      /**
       * Props
       */
      props: Opaque.t,
      /**
       * Internal value
       */
      instance: ref(option(Opaque.t)),
      /**
       * Error value
       */
      error: ref(option(exn)),
      /**
       * Hooks
       */
      hooks: ref(Opaque.Array.t),
      /**
       * Force rendering
       */
      dependencies: Opaque.Set.t,
      shouldUpdate: ref(bool),
    };

    let make = (fiberTag: Tags.Fiber.t, props: 'a): t => {
      constructor: ref(None),
      fiberTag: fiberTag,
      workTag: ref(Tags.Work.None),
      parent: ref(None),
      alternate: ref(None),
      sibling: ref(None),
      child: ref(None),
      key: ref(None),
      index: ref(0),
      props: Opaque.convert(props),
      instance: ref(None),
      error: ref(None),
      map: Opaque.Map.make(),
      hooks: ref(Opaque.Array.make()),
      ref: ref(None),
      dependencies: Opaque.Set.make(),
      shouldUpdate: ref(false),
      children: ref(None),
      indeces: ref(0),
    };

    /**
     * Detaches fiber from any node it is connected to.
     */
    let rec detach = (fiber: option(t)): unit => {
      ignore({
        let%Option actualFiber = fiber;
        let child = actualFiber.child^;
        let sibling = actualFiber.sibling^;
        let alternate = actualFiber.alternate^;

        actualFiber.child := None;
        actualFiber.sibling := None;
        actualFiber.alternate := None;

        detach(alternate);
        detach(child);
        detach(sibling);

        None;
      });
    };
  };

  module Context = {
    type t('a) = {
      defaultValue: 'a,
    };

    module Instance = {
      type t('a) = {
        value: ref('a),
        shouldUpdate: ref(bool),
      };
    };

    module Provider = {
      type props('a) = {
        context: t('a),
        value: option('a),
        children: Element.t,
      };

      let make = ({ key, ref }: Element.base, { context, value, children }: props('a)): option(Element.t) => {
        let actualValue = switch (value) {
          | Some(actual) => actual;
          | None => context.defaultValue;
        };

        Some({
          fiberTag: Tags.Fiber.ContextProvider,
          constructor: None,
          key: key,
          ref: ref,
          props: Opaque.convert({
            context,
            value: Some(actualValue),
            children,
          }),
        });
      };
    };

    module Consumer = {
      type props('a) = {
        context: t('a),
        build: 'a => option(Element.t),
      };

      let make = ({ key, ref }: Element.base, { context, build }: props('a)): option(Element.t) => {
        Some({
          fiberTag: Tags.Fiber.ContextProvider,
          constructor: None,
          key: key,
          ref: ref,
          props: Opaque.convert({
            context,
            build,
          }),
        });
      };
    };


    let rec read = (wip: Fiber.t, context: t('a)): Instance.t('a) => {
      let%OptionOrError { parent, fiberTag, props, instance } = (wip.parent^, Exceptions.MissingContext);

      let actualProps: Provider.props('a) = Opaque.return(props);

      if (fiberTag == Tags.Fiber.ContextProvider && actualProps.context == context) {
        let%OptionOrError actualInstance = (instance^, Exceptions.MissingContext);
        Opaque.return(actualInstance);
      } else {
        let%OptionOrError actualParent = (parent^, Exceptions.MissingContext);
        read(actualParent, context);
      }
    };
  };

  module ErrorBoundary = {
    type props = {
      onError: exn => unit,
      fallback: option(Element.t),
      children: array(option(Element.t)),
    };

    let make = ({ key }: Element.base, props: props): option(Element.t) => {
      Some({
        fiberTag: Tags.Fiber.ErrorBoundary,
        constructor: None,
        key: key,
        ref: None,
        props: Opaque.convert(props),
      });
    };
  };

  module Fragment = {
    type props = {
      children: array(option(Element.t)),
    };

    let make = ({ key }: Element.base, props: props): option(Element.t) => {
      Some({
        fiberTag: Tags.Fiber.Fragment,
        constructor: None,
        key: key,
        ref: None,
        props: Opaque.convert(props),
      });
    };
  };

  module Root = {
    type props = {
      children: array(option(Element.t)),
    };

    let make = ({ key, ref }: Element.base, props: props): option(Element.t) => {
      Some({
        fiberTag: Tags.Fiber.Root,
        constructor: None,
        key: key,
        ref: ref,
        props: Opaque.convert(props),
      });
    };
  };

  module Host = {
    type props = {
      constructor: string,
      attributes: Opaque.t,
      children: option(array(option(Element.t))),
    };

    let make = ({ key, ref }: Element.base, { constructor, attributes, children }: props): option(Element.t) => {
      Some({
        fiberTag: Tags.Fiber.Host,
        constructor: Some(Opaque.convert(constructor)),
        key: key,
        ref: ref,
        props: Opaque.convert({
          constructor,
          attributes,
          children,
        }),
      });
    };
  };

  module Core = {
    let currentRoot: ref(option(Fiber.t)) = ref(None);
    let wipRoot: ref(option(Fiber.t)) = ref(None);
    let nextUnitOfWork: ref(option(Fiber.t)) = ref(None);

    let update = () => {
      let%OptionUnit current = currentRoot^;
      let fiber = Fiber.make(Tags.Fiber.Root, current.props);

      fiber.instance := current.instance^;
      fiber.alternate := current.alternate^;

      wipRoot := Some(fiber);
      nextUnitOfWork := Some(fiber);
    };

    let render = (element: Element.t, container: Reconciler.t) => {
      let props: Root.props = {
        children: [| Some(element) |],
      };

      let fiber = Fiber.make(Tags.Fiber.Root, props);

      fiber.instance := Some(Opaque.convert(container));
      fiber.alternate := currentRoot^;

      wipRoot := Some(fiber);
      nextUnitOfWork := Some(fiber);
    };
  };

  module Utils = {
    let rec getHostParent = (wip: Fiber.t): option(Fiber.t) => {
      let%Option parent = wip.parent^;

      if (parent.fiberTag == Tags.Fiber.Host || parent.fiberTag == Tags.Fiber.Root) {
        Some(parent);
      } else {
        getHostParent(parent);
      }
    };

    let getInstanceIndex = (wip: Fiber.t): int => {
      let index = wip.indeces^;
      wip.indeces := index + 1;
      index;
    };
  };

  module ReconcileChildren = {
    /**
     * Attaches fiber to its alternate
     */
    let attachFiberAlternate = (oldFiber: Fiber.t, newFiber: Fiber.t) => {
      /**
       * Detach old fiber to it's older alternate
       * and set the newest fiber as the alternate
       */
      Fiber.detach(oldFiber.alternate^);
      oldFiber.alternate := None;
      newFiber.alternate := Some(oldFiber);
    };

    /**
     * Maps the new fiber to its parent's key map.
     */
    let mapFiberToParent = (parent: Fiber.t, newFiber: Fiber.t, index: int, key: option(string)) => {
      newFiber.parent := Some(parent);
      /**
       * Set fiber references
       */
      newFiber.index := index;
      newFiber.key := key;

      /**
       * Map current fiber to parent using the key or the index.
       */
      switch (key) {
        | Some(actualKey) => Opaque.Map.set(parent.map, actualKey, newFiber);
        | None => Opaque.Map.set(parent.map, index, newFiber);
      }
    };

    let replaceFiber = (parent: Fiber.t, oldFiber: Fiber.t, element: Element.t, index: int, key: option(string)): Fiber.t => {
      let fiber = Fiber.make(element.fiberTag, element.props);
      fiber.constructor := element.constructor;
      fiber.workTag := Tags.Work.Replace;

      attachFiberAlternate(oldFiber, fiber);
      mapFiberToParent(parent, fiber, index, key);
      
      fiber;
    };

    let deleteFiber = (parent: Fiber.t, oldFiber: Fiber.t, index: int, key: option(string)): Fiber.t => {
      let fiber = Fiber.make(oldFiber.fiberTag, oldFiber.props);
      fiber.constructor := oldFiber.constructor^;
      fiber.workTag := Tags.Work.Delete;
      fiber.instance := oldFiber.instance^;

      attachFiberAlternate(oldFiber, fiber);
      mapFiberToParent(parent, fiber, index, key);
      
      fiber;
    };

    /**
     * Updates fiber from the given element.
     */
    let updateFiberFromElement = (parent: Fiber.t, oldFiber: Fiber.t, element: Element.t, index: int, key: option(string)): Fiber.t => {
      let fiber = Fiber.make(oldFiber.fiberTag, element.props);

      /**
       * Set other fields
       */
      fiber.workTag := Tags.Work.Update;
      fiber.constructor := oldFiber.constructor^;
      fiber.instance := oldFiber.instance^;

      attachFiberAlternate(oldFiber, fiber);
      mapFiberToParent(parent, fiber, index, key);

      fiber;
    };

    /**
     * Create fiber from element
     */
    let createFiberFromElement = (parent: Fiber.t, element: Element.t, index: int, key: option(string)): Fiber.t => {
      let fiber = Fiber.make(element.fiberTag, element.props);

      /**
       * Set other fields
       */
      fiber.workTag := Tags.Work.Placement;
      fiber.constructor := element.constructor;

      mapFiberToParent(parent, fiber, index, key);

      fiber;
    };

    /**
     * Produces a new fiber that represents the new unit of work for the new tree.
     */
    let updateFiber = (parent: Fiber.t, oldFiber: option(Fiber.t), element: option(Element.t), index: int, key: option(string)): option(Fiber.t) => {
      switch (oldFiber, element) {
        | (Some(actualOldFiber), Some(actualElement)) => {
          Some(if (actualOldFiber.workTag^ == Tags.Work.Delete) {
            createFiberFromElement(parent, actualElement, index, key);
          } else if (actualElement.fiberTag != actualOldFiber.fiberTag) {
            replaceFiber(parent, actualOldFiber, actualElement, index, key);
          } else if (actualElement.constructor != actualOldFiber.constructor^) {
            replaceFiber(parent, actualOldFiber, actualElement, index, key);
          } else {
            updateFiberFromElement(parent, actualOldFiber, actualElement, index, key);
          })
        }
        | (Some(actualOldFiber), None) => {
          Some(deleteFiber(parent, actualOldFiber, index, key));
        }
        | (None, Some(actualElement)) => {
          Some(createFiberFromElement(parent, actualElement, index, key));
        }
        | (None, None) => None;
      }
    };

    /**
     * Matches the actual fiber from the given key/index from the parent fiber.
     * 
     * Prioritizes key first before index.
     */
    let getMatchingFiber = (current: option(Fiber.t), index: int, key: option(string)): option(Fiber.t) => {
      let%Option actualCurrent = current;
      switch (key) {
        | (Some(actualKey)) => {
          if (Opaque.Map.has(actualCurrent.map, actualKey)) {
            Opaque.Map.get(actualCurrent.map, actualKey);
          } else {
            Opaque.Map.get(actualCurrent.map, index);
          }
        };
        | (None) => {
          Opaque.Map.get(actualCurrent.map, index);
        };
      }
    };

    let call = (current: option(Fiber.t), wip: Fiber.t, newChildren: option(array(option(Element.t)))): option(Fiber.t) => {
      let previousFiber: ref(option(Fiber.t)) = ref(None);

      /**
       * Connects the newest fiber to the wip tree
       */
      let linkFiber = (newFiber: option(Fiber.t), hasElement: bool): unit => {
        let%OptionUnit fiber = newFiber;
        if (wip.child^ == None) {
          /**
           * Set the newest fiber as the first child of the wip fiber
           * if the wip fiber doesn't have any child.
           */
          wip.child := Some(fiber);
        } else if (hasElement) {
          /**
           * Otherwise, set the new fiber as a sibling from the previously
           * created fiber.
           */
            let%OptionUnit prev = previousFiber^;
            prev.sibling := Some(fiber);
        }

        previousFiber := Some(fiber);
      };

      let marked = Opaque.Set.make();

      /**
       * Perform work to new children
       */
      ignore({
        let%OptionUnit children = newChildren;
        
        children |> Array.iteri((index, element: option(Element.t)) => {
          let key: option(string) = switch (element) {
            | Some(el) => el.key;
            | None => None;
          };

          let oldFiber: option(Fiber.t) = getMatchingFiber(current, index, key);
          let newFiber: option(Fiber.t) = updateFiber(wip, oldFiber, element, index, key);

          ignore({
            let%OptionUnit actualFiber = oldFiber;
            Opaque.Set.add(marked, actualFiber);
          });

          linkFiber(newFiber, element != None);
        });
      })

      /**
       * Perform deletion for remaining children
       */
      ignore({
        let%OptionUnit actualCurrent = current;
        let rec iterateFibers = (oldFiber: option(Fiber.t)) => {
          let%OptionUnit fiber = oldFiber;
          if (!Opaque.Set.has(marked, fiber)) {
            linkFiber(Some(deleteFiber(wip, fiber, fiber.index^, fiber.key^)), false);
          }
          iterateFibers(fiber.sibling^);
        };

        iterateFibers(actualCurrent.child^);
      })

      wip.child^;
    };
  };

  module HooksContext = {
    let hookFiber: ref(option(Fiber.t)) = ref(None);
    let hookCursor: ref(int) = ref(0);

    let render = (current: option(Fiber.t), wip: Fiber.t) => {
      hookFiber := Some(wip);
      hookCursor := 0;

      let%OptionUnit actualCurrent = current;
      wip.hooks := actualCurrent.hooks^;
      actualCurrent.hooks := Opaque.Array.make();
    };

    let finishRender = () => {
      hookFiber := None;
      hookCursor := 0;
    };

    let getCurrentFiber = (): Fiber.t => {
      let%OptionOrError fiber = (hookFiber^, Exceptions.OutOfContextHookCall);
      fiber;
    };

    let make = (tag: Tags.Hook.t): Hooks.t => {
      let fiber = getCurrentFiber();
      hookCursor := hookCursor^ + 1;

      let slot: option(Hooks.t) = Opaque.Array.get(fiber.hooks^, hookCursor^);

      switch (slot) {
        | Some(actualSlot) => {
          if (actualSlot.tag != tag) {
            raise(Exceptions.IncompatibleHook);
          }
          actualSlot;
        };
        | None => {
          let newSlot: Hooks.t = {
            tag: tag,
            value: ref(None),
          };

          Opaque.Array.set(fiber.hooks^, hookCursor^, newSlot);

          newSlot;
        };
      }
    };

    let forEach = (wip: Fiber.t, handler: Hooks.t => unit): unit => {
      Opaque.Array.forEach(wip.hooks^, (slot, _) => {
        handler(slot);
      });
    };

    let clear = (fiber: Fiber.t) => {
      fiber.hooks := Opaque.Array.make();
    };
  };

  module HooksFunction = {
    /**
     * Memoizes a callback reference.
     * 
     * Updates the reference if the dependency changes.
     */
    module Callback = {
      type t('i, 'o) = 'i => 'o;

      let use = (callback: t('i, 'o), dependency: 'dependency): t('i, 'o) => {
        let state = HooksContext.make(Tags.Hook.Callback);
        let dep = HooksContext.make(Tags.Hook.Dependency);

        switch (state.value^) {
          | Some(actualCallback) => {
            if (dep.value^ != Some(Opaque.convert(dependency))) {
              state.value := Some(Opaque.convert(callback));
              dep.value := Some(Opaque.convert(dependency));
              callback;
            } else {
              Opaque.return(actualCallback);
            }
          };
          | None => {
            state.value := Some(Opaque.convert(callback));
            dep.value := Some(Opaque.convert(dependency));
            callback;
          }
        }
      };
    };

    /**
     * Creates a component-level constant.
     */
    module Constant = {
      let use = (supplier: unit => 'value): 'value => {
        let state = HooksContext.make(Tags.Hook.Constant);

        switch (state.value^) {
          | Some(value) => Opaque.return(value);
          | None => {
            let value = supplier();
            state.value := Some(Opaque.convert(value));
            value;
          };
        }
      };
    };

    /**
     * Attaches context dependency
     */
    module Context = {
      let use = (context: Context.t('a)): 'a => {
        let wip = HooksContext.getCurrentFiber();

        let instance = Context.read(wip, context);

        Opaque.Set.add(wip.dependencies, context);

        instance.value^;
      };
    };

    /**
     * Attaches a side-effect to the component that runs
     * after mutations and every time the dependency changes.
     */
    module Effect = {
      type cleanup = unit => unit;
      type effect = unit => option(cleanup);

      let use = (effect: effect, dependency: 'dependency): unit => {
        /**
         * Create hooks for effect and dep
         */
        let state = HooksContext.make(Tags.Hook.Effect);
        let dep = HooksContext.make(Tags.Hook.Dependency);

        switch (dep.value^) {
          | Some(actualDep) => {
            /**
             * Compare dependency
             */
            if (actualDep != dependency) {
              /**
               * Get previous slot value
               */
              let%OptionUnit opaquePrev = state.value^;
              let (effect, cleanup, _) = Opaque.return(opaquePrev);
              /**
               * Update slot value
               */
              state.value := Some(Opaque.convert((
                effect,
                cleanup,
                Tags.Work.Update,
              )));
              /**
               * Update dependency value
               */
              dep.value := Some(Opaque.convert(dependency));
            }
          };
          | None => {
            state.value := Some(Opaque.convert((
              effect,
              ref(None),
              Tags.Work.Placement,
            )));
            dep.value := Some(Opaque.convert(dependency));
          };
        }
      };
    };

    /**
     * Creates a stable function that allows force updates.
     */
    module ForceUpdate = {
      let use = () => {
        let dispatch = HooksContext.make(Tags.Hook.ForceUpdate);

        switch (dispatch.value^) {
          | Some(actualValue) => Opaque.return(actualValue);
          | None => {
            let callback = () => {
              Core.update();
            };

            dispatch.value := Some(Opaque.convert(callback));

            callback;
          };
        }
      };
    };

    /**
     * Attaches a side-effect to the component that runs
     * before mutations and every time the dependency changes.
     */
    module LayoutEffect = {
      type cleanup = unit => unit;
      type effect = unit => option(cleanup);

      let use = (effect: effect, dependency: 'dependency): unit => {
        /**
         * Create hooks for effect and dep
         */
        let state = HooksContext.make(Tags.Hook.LayoutEffect);
        let dep = HooksContext.make(Tags.Hook.Dependency);

        switch (dep.value^) {
          | Some(actualDep) => {
            /**
             * Compare dependency
             */
            if (actualDep != dependency) {
              /**
               * Get previous slot value
               */
              let%OptionUnit opaquePrev = state.value^;
              let (effect, cleanup, _) = Opaque.return(opaquePrev);
              /**
               * Update slot value
               */
              state.value := Some(Opaque.convert((
                effect,
                cleanup,
                Tags.Work.Update,
              )));
              /**
               * Update dependency value
               */
              dep.value := Some(Opaque.convert(dependency));
            }
          };
          | None => {
            state.value := Some(Opaque.convert((
              effect,
              ref(None),
              Tags.Work.Placement,
            )));
            dep.value := Some(Opaque.convert(dependency));
          };
        }
      };
    };

    /**
     * Creates a component-level memoization that recomputes
     * whenever the dependency change.
     */
    module Memo = {
      let use = (supplier: unit => 'value, dependency: 'dependency): 'value => {
        let state = HooksContext.make(Tags.Hook.Memo);
        let dep = HooksContext.make(Tags.Hook.Dependency);

        switch (state.value^) {
          | Some(actualState) => {
            if (dep.value^ != Some(Opaque.convert(dependency))) {
              let value = supplier();
              state.value := Some(Opaque.convert(value));
              dep.value := Some(Opaque.convert(dependency));
              value;
            } else {
              Opaque.return(actualState);
            }
          };
          | None => {
            let value = supplier();
            state.value := Some(Opaque.convert(value));
            dep.value := Some(Opaque.convert(dependency));
            value;
          };
        }
      };
    };

    /**
     * Creates a reducer-based state
     */
    module Reducer = {
      let use = (reducer: ('state, 'action) => 'state, initial: unit => 'state): ('state, 'action => unit) => {
        let wip = HooksContext.getCurrentFiber();
        let state = HooksContext.make(Tags.Hook.ReducerState);
        let dispatch = HooksContext.make(Tags.Hook.ReducerDispatch);

        switch (state.value^, dispatch.value^) {
          | (Some(actualState), Some(actualDispatch)) => {
            (Opaque.return(actualState), Opaque.return(actualDispatch));
          };
          | (_, _) => {
            let initialState = initial();
            let callback = (action: 'action) => {
              let%OptionUnit opaqueValue = state.value^;
              let actualValue = Opaque.return(opaqueValue);
              let newState = reducer(actualValue, action);

              if (newState != actualValue) {
                state.value := Some(Opaque.convert(newState));
                wip.shouldUpdate := true;
                Core.update();
              }
            };

            state.value := Some(Opaque.convert(initialState));
            dispatch.value := Some(Opaque.convert(callback));

            (initialState, callback);
          };
        }
      };
    };
  };

  module BeginWork = {
    let safelyRender = (wip: Fiber.t, render: unit => option(Element.t)): option(Element.t) => {
      switch (render()) {
        | result => result;
        | exception e => {
          wip.error := Some(e);
          None;
        };
      }
    };

    let updateBasic = (current: option(Fiber.t), wip: Fiber.t): option(Fiber.t) => {
      let result: option(Element.t) = safelyRender(wip, () => {
        let%OptionOrError constructor = (wip.constructor^, Exceptions.MissingBasicComponentConstructor);

        let render: (Element.base, 'props) => option(Element.t) = Opaque.return(constructor);
        let props: 'props = Opaque.return(wip.props);
        render({
          key: wip.key^,
          ref: wip.ref^,
        }, props);
      });

      if (result != None) {
        ReconcileChildren.call(current, wip, Some([| result |]));
      } else {
        None;
      }
    };

    let updateComponent = (current: option(Fiber.t), wip: Fiber.t): option(Fiber.t) => {
      HooksContext.render(current, wip);
      let result: option(Element.t) = safelyRender(wip, () => {
        let%OptionOrError constructor = (wip.constructor^, Exceptions.MissingBasicComponentConstructor);

        let render: (Element.base, 'props) => option(Element.t) = Opaque.return(constructor);
        let props: 'props = Opaque.return(wip.props);
        render(props, {
          key: wip.key^,
          ref: wip.ref^,
        });
      });
      HooksContext.finishRender();

      if (result != None) {
        ReconcileChildren.call(current, wip, Some([| result |]));
      } else {
        None;
      }
    };

    let updateContextConsumer = (current: option(Fiber.t), wip: Fiber.t): option(Fiber.t) => {
      let props: Context.Consumer.props('a) = Opaque.return(wip.props);
      
      let result: option(Element.t) = safelyRender(wip, () => {
        let instance = Context.read(wip, props.context);

        let children: option(Element.t) = if (instance.shouldUpdate^) {
          props.build(instance.value^);
        } else {
          let%Option actualCurrent = current;
          let result = actualCurrent.children^;
          actualCurrent.children := None;
          result;
        };

        wip.children := children;

        children;
      });

      if (result != None) {
        ReconcileChildren.call(current, wip, Some([| result |]));
      } else {
        None;
      }
    };

    let updateContextProvider = (current: option(Fiber.t), wip: Fiber.t): option(Fiber.t) => {
      let props: Context.Provider.props('a) = Opaque.return(wip.props);
      let%OptionOrError actualValue = (props.value, Exceptions.DesyncContextValue);

      switch (wip.instance^) {
        | None => {
          let instance: Context.Instance.t('a) = {
            value: ref(actualValue),
            shouldUpdate: ref(true),
          };
          wip.instance := Some(Opaque.convert(instance));
        };
        | Some(actualInstance) => {
          let instance: Context.Instance.t('a) = Opaque.return(actualInstance);
          let%OptionOrError actualCurrent = (current, Exceptions.UnboundContextInstance);
          let%OptionOrError prevOpaqueInstance = (actualCurrent.instance^, Exceptions.UnboundContextInstance);
          let prevInstance: Context.Instance.t('a) = Opaque.return(prevOpaqueInstance);
          instance.shouldUpdate := actualValue != prevInstance.value^;
          instance.value := actualValue;
        };
      }

      ReconcileChildren.call(current, wip, Some([| Some(props.children) |]));
    };

    let updateErrorBoundary = (current: option(Fiber.t), wip: Fiber.t): option(Fiber.t) => {
      let props: ErrorBoundary.props = Opaque.return(wip.props);

      ReconcileChildren.call(current, wip, Some(props.children));
    };

    let updateFragment = (current: option(Fiber.t), wip: Fiber.t): option(Fiber.t) => {
      let props: Fragment.props = Opaque.return(wip.props);

      ReconcileChildren.call(current, wip, Some(props.children));
    };

    let updateHost = (current: option(Fiber.t), wip: Fiber.t): option(Fiber.t) => {
      let props: Host.props = Opaque.return(wip.props);
      if (wip.instance^ == None) {
        let%OptionOrError constructor = (wip.constructor^, Exceptions.InvalidHostConstructor);
        let stringConstructor: string = Opaque.return(constructor);
        let instance = Reconciler.createInstance(stringConstructor, props.attributes);
        wip.instance := Some(Opaque.convert(instance));
      }
      ReconcileChildren.call(current, wip, props.children);
    };

    let updateMemoInitial = (current: option(Fiber.t), wip: Fiber.t): option(Fiber.t) => {
      HooksContext.render(current, wip);
      let result: option(Element.t) = safelyRender(wip, () => {
        let%OptionOrError constructor = (wip.constructor^, Exceptions.MissingBasicComponentConstructor);

        let render: (Element.base, 'props) => option(Element.t) = Opaque.return(constructor);
        let props: 'props = Opaque.return(wip.props);
        render({
          key: wip.key^,
          ref: wip.ref^,
        }, props);
      });
      HooksContext.finishRender();

      if (result != None) {
        wip.children := result;
        ReconcileChildren.call(current, wip, Some([| result |]));
      } else {
        None;
      }
    };

    let updateMemo = (current: option(Fiber.t), wip: Fiber.t): option(Fiber.t) => {
      switch (current) {
        | Some(actualCurrent) => {
          let shouldUpdate = ref(actualCurrent.shouldUpdate^);

          if (!shouldUpdate^) {
            let deps = actualCurrent.dependencies;

            Opaque.Set.forEach(deps, (contextType) => {
              let instance = Context.read(wip, contextType);

              if (instance.shouldUpdate^) {
                shouldUpdate := true;
              }
            });
          }

          let currentProps: 'propsA = Opaque.return(actualCurrent.props);
          let wipProps: 'propsB = Opaque.return(wip.props);

          if (shouldUpdate^ || (wipProps != currentProps)) {
            updateMemoInitial(current, wip);
          } else {
            let children = actualCurrent.children^;
            wip.children := children;
            actualCurrent.children := None;
            ReconcileChildren.call(current, wip, Some([| children |]));
          }
        };
        | None => updateMemoInitial(current, wip);
      }
    };

    let updateMemoBasicInitial = (current: option(Fiber.t), wip: Fiber.t): option(Fiber.t) => {
      let result: option(Element.t) = safelyRender(wip, () => {
        let%OptionOrError constructor = (wip.constructor^, Exceptions.MissingBasicComponentConstructor);

        let render: (Element.base, 'props) => option(Element.t) = Opaque.return(constructor);
        let props: 'props = Opaque.return(wip.props);
        render({
          key: wip.key^,
          ref: wip.ref^,
        }, props);
      });

      if (result != None) {
        wip.children := result;
        ReconcileChildren.call(current, wip, Some([| result |]));
      } else {
        None;
      }
    };

    let updateMemoBasic = (current: option(Fiber.t), wip: Fiber.t): option(Fiber.t) => {
      switch (current) {
        | Some(actualCurrent) => {
          let currentProps: 'propsA = Opaque.return(actualCurrent.props);
          let wipProps: 'propsB = Opaque.return(wip.props);

          if (wipProps != currentProps) {
            updateMemoInitial(current, wip);
          } else {
            let children = actualCurrent.children^;
            wip.children := children;
            actualCurrent.children := None;
            ReconcileChildren.call(current, wip, Some([| children |]));
          }
        };
        | None => updateMemoInitial(current, wip);
      }
    };

    let updateRoot = (current: option(Fiber.t), wip: Fiber.t): option(Fiber.t) => {
      let props: Fragment.props = Opaque.return(wip.props);

      ReconcileChildren.call(current, wip, Some(props.children));
    };

    let call = (current: option(Fiber.t), wip: Fiber.t): option(Fiber.t) => {
      switch (wip.fiberTag) {
        | Tags.Fiber.Basic => updateBasic(current, wip);
        | Tags.Fiber.Component => updateComponent(current, wip);
        | Tags.Fiber.ContextConsumer => updateContextConsumer(current, wip);
        | Tags.Fiber.ContextProvider => updateContextProvider(current, wip);
        | Tags.Fiber.ErrorBoundary => updateErrorBoundary(current, wip);
        | Tags.Fiber.Fragment => updateFragment(current, wip);
        | Tags.Fiber.Host => updateHost(current, wip);
        | Tags.Fiber.Memo => updateMemo(current, wip);
        | Tags.Fiber.MemoBasic => updateMemoBasic(current, wip);
        | Tags.Fiber.Root => updateRoot(current, wip);
      }
    };
  }

  module Commit = {
    module Error = {
      let call = (wip: Fiber.t) => {
        if (wip.fiberTag == Tags.Fiber.ErrorBoundary) {
          let%OptionUnit error = wip.error^;
          let props: ErrorBoundary.props = Opaque.return(wip.props);

          props.onError(error);
        }
      };
    };

    module Placement = {
      let withHost = (wip: Fiber.t): unit => {
        let%OptionUnit parent = Utils.getHostParent(wip);
        let%OptionUnit parentInstance = parent.instance^;
        let%OptionUnit childInstance = wip.instance^;

        Reconciler.appendChild(
          Opaque.return(parentInstance),
          Opaque.return(childInstance),
          Utils.getInstanceIndex(parent),
        );
      };

      type tuple = (
        HooksFunction.LayoutEffect.effect,
        ref(option(HooksFunction.LayoutEffect.cleanup)),
        Tags.Work.t,
      );

      let withHooks = (wip: Fiber.t): unit => {
        Opaque.Array.forEach(wip.hooks^, (hook, _) => {
          let { tag, value }: Hooks.t = Opaque.return(hook);

          if (tag == Tags.Hook.LayoutEffect) {
            let%OptionUnit actualValue = value^;
            let (effect, cleanup, workTag): tuple = Opaque.return(actualValue);

            if (workTag == Tags.Work.Placement) {
              cleanup := effect();
            }
          }
        });
      };

      let call = (wip: Fiber.t): unit => {
        switch (wip.fiberTag) {
          | Tags.Fiber.Host => withHost(wip);
          | Tags.Fiber.Component => withHooks(wip);
          | Tags.Fiber.Memo => withHooks(wip);
          | _ => ();
        }
      };
    };

    module Update = {
      let withHost = (wip: Fiber.t): unit => {
        let%OptionUnit parent = Utils.getHostParent(wip);
        let%OptionUnit childInstance = wip.instance^;
        let%OptionUnit alternate = wip.alternate^;

        Reconciler.commitUpdate(
          Opaque.return(childInstance),
          Opaque.return(alternate.props),
          Opaque.return(wip.props),
          Utils.getInstanceIndex(parent),
        );
      };

      type tuple = (
        HooksFunction.LayoutEffect.effect,
        ref(option(HooksFunction.LayoutEffect.cleanup)),
        Tags.Work.t,
      );

      let withHooks = (wip: Fiber.t): unit => {
        Opaque.Array.forEach(wip.hooks^, (hook, _) => {
          let { tag, value }: Hooks.t = Opaque.return(hook);

          if (tag == Tags.Hook.LayoutEffect) {
            let%OptionUnit actualValue = value^;
            let (effect, cleanup, workTag): tuple = Opaque.return(actualValue);

            if (workTag == Tags.Work.Update) {
              ignore({
                let%OptionUnit actualCleanup = cleanup^;
                actualCleanup();
              });
              cleanup := effect();
            }
          }
        });
      };

      let call = (wip: Fiber.t): unit => {
        switch (wip.fiberTag) {
          | Tags.Fiber.Host => withHost(wip);
          | Tags.Fiber.Component => withHooks(wip);
          | Tags.Fiber.Memo => withHooks(wip);
          | _ => ();
        }
      };
    };

    module Delete = {
      let withHost = (wip: Fiber.t): unit => {
        let%OptionUnit parent = Utils.getHostParent(wip);
        let%OptionUnit parentInstance = parent.instance^;
        let%OptionUnit childInstance = wip.instance^;

        Reconciler.removeChild(
          Opaque.return(parentInstance),
          Opaque.return(childInstance),
          Utils.getInstanceIndex(parent),
        );
      };

      type tuple = (
        HooksFunction.LayoutEffect.effect,
        ref(option(HooksFunction.LayoutEffect.cleanup)),
        Tags.Work.t,
      );

      let withHooks = (wip: Fiber.t): unit => {
        Opaque.Array.forEach(wip.hooks^, (hook, _) => {
          let { tag, value }: Hooks.t = Opaque.return(hook);

          if (tag == Tags.Hook.LayoutEffect) {
            let%OptionUnit actualValue = value^;
            let (_, cleanup, _): tuple = Opaque.return(actualValue);
            let%OptionUnit actualCleanup = cleanup^;
            actualCleanup();
          }
        });
      };

      let commitDelete = (wip: Fiber.t): unit => {
        switch (wip.fiberTag) {
          | Tags.Fiber.Host => withHost(wip);
          | Tags.Fiber.Component => withHooks(wip);
          | Tags.Fiber.Memo => withHooks(wip);
          | _ => ();
        }
      };

      let rec commitDeleteSibling = (wip: option(Fiber.t)) => {
        let%OptionUnit actualFiber = wip;
        commitDelete(actualFiber);
        commitDeleteSibling(actualFiber.sibling^);
      };

      let call = (wip: Fiber.t) => {
        commitDelete(wip);
        commitDeleteSibling(wip.child^);
      };
    };

    module Lifecycles = {
      module Placement = {
        type tuple = (
          HooksFunction.Effect.effect,
          ref(option(HooksFunction.Effect.cleanup)),
          Tags.Work.t,
        );

        let withHooks = (wip: Fiber.t): unit => {
          Opaque.Array.forEach(wip.hooks^, (hook, _) => {
            let { tag, value }: Hooks.t = Opaque.return(hook);

            if (tag == Tags.Hook.Effect) {
              let%OptionUnit actualValue = value^;
              let (effect, cleanup, workTag): tuple = Opaque.return(actualValue);

              if (workTag == Tags.Work.Placement) {
                cleanup := effect();
              }
            }
          });
        };

        let call = (wip: Fiber.t): unit => {
          switch (wip.fiberTag) {
            | Tags.Fiber.Component => withHooks(wip);
            | Tags.Fiber.Memo => withHooks(wip);
            | _ => ();
          }
        };
      };

      module Update = {
        type tuple = (
          HooksFunction.Effect.effect,
          ref(option(HooksFunction.Effect.cleanup)),
          Tags.Work.t,
        );

        let withHooks = (wip: Fiber.t): unit => {
          Opaque.Array.forEach(wip.hooks^, (hook, _) => {
            let { tag, value }: Hooks.t = Opaque.return(hook);

            if (tag == Tags.Hook.Effect) {
              let%OptionUnit actualValue = value^;
              let (effect, cleanup, workTag): tuple = Opaque.return(actualValue);

              if (workTag == Tags.Work.Update) {
                ignore({
                  let%OptionUnit actualCleanup = cleanup^;
                  actualCleanup();
                });
                cleanup := effect();
              }
            }
          });
        };

        let call = (wip: Fiber.t): unit => {
          switch (wip.fiberTag) {
            | Tags.Fiber.Component => withHooks(wip);
            | Tags.Fiber.Memo => withHooks(wip);
            | _ => ();
          }
        };
      };

      module Delete = {
        type tuple = (
          HooksFunction.Effect.effect,
          ref(option(HooksFunction.Effect.cleanup)),
          Tags.Work.t,
        );

        let withHooks = (wip: Fiber.t): unit => {
          Opaque.Array.forEach(wip.hooks^, (hook, _) => {
            let { tag, value }: Hooks.t = Opaque.return(hook);

            if (tag == Tags.Hook.Effect) {
              let%OptionUnit actualValue = value^;
              let (_, cleanup, _): tuple = Opaque.return(actualValue);
              let%OptionUnit actualCleanup = cleanup^;
              actualCleanup();
            }
          });
        };

        let commitDelete = (wip: Fiber.t): unit => {
          switch (wip.fiberTag) {
            | Tags.Fiber.Component => withHooks(wip);
            | Tags.Fiber.Memo => withHooks(wip);
            | _ => ();
          }
        };

        let rec commitDeleteSibling = (wip: option(Fiber.t)) => {
          let%OptionUnit actualFiber = wip;
          commitDelete(actualFiber);
          commitDeleteSibling(actualFiber.sibling^);
        };

        let call = (wip: Fiber.t) => {
          commitDelete(wip);
          commitDeleteSibling(wip.child^);
        };
      };

      module Work = {
        let safelyCommit = (wip: Fiber.t, commit: Fiber.t => unit, alternate: option(Fiber.t)) => {
          let commitAny = () => {
            switch (alternate) {
              | None => commit(wip);
              | Some(alt) => commit(alt);
            }
          };

          switch (commitAny()) {
            | () => ();
            | exception e => wip.error := Some(e);
          }
        };

        let rec call = (wip: option(Fiber.t)): unit => {
          let%OptionUnit fiber = wip;

          let commitOnChild = ref(true);

          switch (fiber.workTag^) {
            | Tags.Work.Placement => safelyCommit(fiber, Placement.call, None);
            | Tags.Work.Update => safelyCommit(fiber, Update.call, None);
            | Tags.Work.Delete => {
              safelyCommit(fiber, Delete.call, fiber.alternate^);
              commitOnChild := false;
            }
            | Tags.Work.Replace => {
              safelyCommit(fiber, Delete.call, fiber.alternate^);
              safelyCommit(fiber, Placement.call, None);
            };
            | _ => ();
          }

          if (fiber.error^ != None) {
            Error.call(fiber);
          } else if (commitOnChild^) {
            call(fiber.child^);
          }

          call(fiber.sibling^);
        };
      };
    };

    module Work = {
      let safelyCommit = (wip: Fiber.t, commit: Fiber.t => unit, alternate: option(Fiber.t)) => {
        let commitAny = () => {
          switch (alternate) {
            | None => commit(wip);
            | Some(alt) => commit(alt);
          }
        };

        switch (commitAny()) {
          | () => ();
          | exception e => wip.error := Some(e);
        }
      };

      let rec call = (wip: option(Fiber.t)): unit => {
        let%OptionUnit fiber = wip;

        let commitOnChild = ref(true);

        switch (fiber.workTag^) {
          | Tags.Work.Placement => safelyCommit(fiber, Placement.call, None);
          | Tags.Work.Update => safelyCommit(fiber, Update.call, None);
          | Tags.Work.Delete => {
            safelyCommit(fiber, Delete.call, fiber.alternate^);
            commitOnChild := false;
          }
          | Tags.Work.Replace => {
            safelyCommit(fiber, Delete.call, fiber.alternate^);
            safelyCommit(fiber, Placement.call, None);
          };
          | _ => ();
        }

        if (fiber.error^ != None) {
          Error.call(fiber);
        } else if (commitOnChild^) {
          call(fiber.child^);
        }

        call(fiber.sibling^);
      };
    };

    module Root = {
      let call = () => {
        ignore({
          let%OptionUnit wip = Core.wipRoot^;
          Work.call(wip.child^);
        });
        ignore({
          let%OptionUnit current = Core.currentRoot^;
          Fiber.detach(current.alternate^);
        });
        Core.currentRoot := Core.wipRoot^;
        Core.wipRoot := None;
        ignore({
          let%OptionUnit current = Core.currentRoot^;
          Work.call(current.child^);
        });
      };
    };
  };

  let rec completeUnitOfWork = (wip: option(Fiber.t)): option(Fiber.t) => {
    let%Option actualWIP = wip;
    let sibling = actualWIP.sibling^;

    if (sibling == None) {
      completeUnitOfWork(actualWIP.parent^);
    } else {
      sibling;
    }
  };

  let performUnitOfWork = (current: option(Fiber.t), wip: Fiber.t): option(Fiber.t) => {
    let nextWork = BeginWork.call(current, wip);

    if (nextWork == None) {
      completeUnitOfWork(Some(wip));
    } else {
      nextWork;
    }
  }

  let workLoop = (deadline: unit => float) => {
    let shouldYield = ref(false);

    let rec loop = (deadline: unit => float) => {
      if (!shouldYield^) {
        let%OptionUnit nextUnitOfWork = Core.nextUnitOfWork^

        Core.nextUnitOfWork := performUnitOfWork(
          nextUnitOfWork.alternate^,
          nextUnitOfWork,
        );

        shouldYield := deadline() < 1.0;

        loop(deadline);
      }
    };

    loop(deadline);

    if (Core.nextUnitOfWork^ == None && Core.wipRoot^ != None) {
      Commit.Root.call();
    }
  };
};
