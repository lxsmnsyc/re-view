/**
 * @license
 * MIT License
 *
 * Copyright (c) 2020 Alexis Munsayac
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *
 * @author Alexis Munsayac <alexis.munsayac@gmail.com>
 * @copyright Alexis Munsayac 2020
 */
open Reconciler;
open Let;

module Make = (Reconciler: ReconcilerType) => {
  module Fiber = {
    let index = ref(0);

    type t = {
      /**
       * fiber name
       */
      name: string,
      /**
       * Fiber constructor
       */
      mutable constructor: option(Opaque.t),
      /**
       * Fiber tag
       */
      fiberTag: Types.Tags.Fiber.t,
      fiberStringTag: string,
      /**
       * Work tag
       */
      mutable workTag: Types.Tags.Work.t,
      mutable workStringTag: string,
      /**
       * Pointers
       */
      mutable parent: option(t),
      mutable alternate: option(t),
      mutable sibling: option(t),
      mutable child: option(t),
      mutable children: option(Types.Element.t),
      /**
       * Mapping
       */
      mutable identifier: string,
      mutable key: option(string),
      mutable index: int,
      map: Opaque.Map.t,
      /**
       * Reference
       */
      mutable ref: Types.Reference.t(Opaque.t),
      /**
       * Props
       */
      props: Opaque.t,
      /**
       * Internal value
       */
      mutable instance: option(Opaque.t),
      /**
       * Hooks
       */
      mutable hooks: Opaque.Array.t,
      /**
       * Force rendering
       */
      dependencies: Opaque.Set.t,
      mutable shouldUpdate: bool,
      /**
       * Error
       */
      mutable error: option(exn),
    };

    let make = (name: string, fiberTag: Types.Tags.Fiber.t, props: 'a): t => {
      name: name,
      constructor: None,
      fiberTag: fiberTag,
      fiberStringTag: Types.Tags.Fiber.map(fiberTag),
      workTag: Types.Tags.Work.None,
      workStringTag: "None",
      parent: None,
      alternate: None,
      sibling: None,
      child: None,
      key: None,
      index: 0,
      props: Opaque.convert(props),
      instance: None,
      map: Opaque.Map.make(),
      hooks: Opaque.Array.make(),
      ref: Types.Reference.None,
      dependencies: Opaque.Set.make(),
      shouldUpdate: false,
      children: None,
      identifier: "",
      error: None,
    };

    /**
     * Detaches fiber from any node it is connected to.
     */
    let rec detach = (detachingFiber: option(t)): unit => {
      ignore({
        let%Option actualFiber = detachingFiber;
        let child = actualFiber.child;
        let sibling = actualFiber.sibling;
        let alternate = actualFiber.alternate;

        actualFiber.child = None;
        actualFiber.sibling = None;
        actualFiber.alternate = None;
        actualFiber.constructor = None;
        actualFiber.workTag = None;
        actualFiber.workStringTag = "None";
        actualFiber.parent = None;
        actualFiber.children = None;
        actualFiber.instance = None;

        detach(alternate);
        detach(child);
        detach(sibling);

        None;
      });
    };

    let createIndex = (): int => {
      let currentIndex = index^ + 1;
      index := currentIndex;
      currentIndex;
    };
  };

  module Context = {
    type t('a) = {
      name: string,
      defaultValue: 'a,
    };

    let make = (name: string, defaultValue: 'a) => ({ name, defaultValue });

    module Instance = {
      type t('a) = {
        mutable value: 'a,
        mutable shouldUpdate: bool,
      };
    };

    module Provider = {
      type props('a) = {
        context: t('a),
        value: option('a),
        children: Types.Children.t,
      };

      let make: Types.Component.t(props('a)) = ({ key, ref }, { context, value, children }) => {
        let actualValue = switch (value) {
          | Some(actual) => actual;
          | None => context.defaultValue;
        };

        Some({
          name: context.name ++ ".Provider",
          fiberTag: Types.Tags.Fiber.ContextProvider,
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
        build: 'a => option(Types.Element.t),
      };

      let make: Types.Component.t(props('a)) = ({ key, ref }, { context, build }) => {
        Some({
          name: context.name ++ ".Consumer",
          fiberTag: Types.Tags.Fiber.ContextConsumer,
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
      let%OptionOrError { parent, fiberTag, props, instance } = (wip.parent, Exceptions.MissingContext);

      let actualProps: Provider.props('a) = Opaque.return(props);

      if (fiberTag == Types.Tags.Fiber.ContextProvider && actualProps.context == context) {
        let%OptionOrError actualInstance = (instance, Exceptions.MissingContext);
        Opaque.return(actualInstance);
      } else {
        let%OptionOrError actualParent = (parent, Exceptions.MissingContext);
        read(actualParent, context);
      }
    };
  };

  module ErrorBoundary = {
    type props = {
      onError: (exn, array(string)) => unit,
      children: Types.Children.t,
    };

    let make: Types.Component.t(props) = ({ key }, props) => {
      Some({
        name: "ErrorBoundary",
        fiberTag: Types.Tags.Fiber.ErrorBoundary,
        constructor: None,
        key: key,
        ref: None,
        props: Opaque.convert(props),
      });
    };
  };

  module Fragment = {
    type props = {
      children: Types.Children.t,
    };

    let make: Types.Component.t(props) = ({ key }, props) => {
      Some({
        name: "Fragment",
        fiberTag: Types.Tags.Fiber.Fragment,
        constructor: None,
        key: key,
        ref: None,
        props: Opaque.convert(props),
      });
    };
  };

  module Root = {
    type props = {
      value: Reconciler.t,
      children: option(Types.Element.t),
    };
  };

  module Host = {
    type props = {
      constructor: string,
      attributes: Opaque.t,
      children: Types.Children.t,
    };

    let make: Types.Component.t(props) = ({ key, ref }, { constructor, attributes, children }) => {
      Some({
        name: constructor,
        fiberTag: Types.Tags.Fiber.Host,
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
  }

  module Basic = (C: Types.Component) => {
    include C;

    let make: Types.Component.t(props) = ({ key, ref }, props) => {
      Some({
        name: C.name,
        fiberTag: Types.Tags.Fiber.Basic,
        constructor: Some(Opaque.convert(C.make)),
        key: key,
        ref: ref,
        props: Opaque.convert(props),
      });
    };
  };

  module MemoBasic = (C: Types.Component) => {
    include C;

    let make: Types.Component.t(props) = ({ key, ref }, props) => {
      Some({
        name: C.name,
        fiberTag: Types.Tags.Fiber.MemoBasic,
        constructor: Some(Opaque.convert(C.make)),
        key: key,
        ref: ref,
        props: Opaque.convert(props),
      });
    };
  };

  module Memo = (C: Types.Component) => {
    include C;

    let make: Types.Component.t(props) = ({ key, ref }, props) => {
      Some({
        name: C.name,
        fiberTag: Types.Tags.Fiber.Memo,
        constructor: Some(Opaque.convert(C.make)),
        key: key,
        ref: ref,
        props: Opaque.convert(props),
      });
    };
  };


  module Component = (C: Types.Component) => {
    include C;

    let make: Types.Component.t(props) = ({ key, ref }, props) => {
      Some({
        name: C.name,
        fiberTag: Types.Tags.Fiber.Component,
        constructor: Some(Opaque.convert(C.make)),
        key: key,
        ref: ref,
        props: Opaque.convert(props),
      });
    };
  };

  module Core = {
    type t = {
      mutable current: option(Fiber.t),
      mutable wip: option(Fiber.t),
      mutable next: option(Fiber.t),
    };

    let root: t = {
      current: None,
      wip: None,
      next: None,
    };

    let rootContainer: ref(option(Opaque.t)) = ref(None);
    let updateScheduled: ref(bool) = ref(false);

    let renderBase = (props: 'a) => {
      let renderFiber = Fiber.make("Root", Types.Tags.Fiber.Root, props);

      renderFiber.instance = rootContainer^;
      renderFiber.alternate = root.current;

      root.wip = Some(renderFiber);
      root.next = Some(renderFiber);
    };

    let update = () => {
      renderBase(switch (root.current) {
        | Some(current) => current.props;
        | None => switch (root.wip) {
          | Some(current) => current.props;
          | None => raise(Exceptions.MissingCurrentRoot);
        }
      });
    };

    let render = (element: option(Types.Element.t), container: Reconciler.t) => {
      rootContainer := Some(Opaque.convert(container));
      let props: Root.props = {
        value: container,
        children: element,
      };
      renderBase(props);
    };
  };

  module Utils = {
    /**
     * Get the nearest parent node that is a root or a host instance
     */
    let rec getHostParent = (wip: Fiber.t): option(Fiber.t) => {
      let%Option parent = wip.parent;

      if (parent.fiberTag == Types.Tags.Fiber.Host || parent.fiberTag == Types.Tags.Fiber.Root) {
        Some(parent);
      } else {
        getHostParent(parent);
      }
    };

    /**
     * Measure the instance's child index in relation to it's parent.
     */
    let getInstanceIndex = (parent: Fiber.t, wip: Fiber.t): int => {
      let count = ref(0);
      let found = ref(false);

      let rec measure = (currentFiber: option(Fiber.t)) => {
        let%OptionUnit current = currentFiber;
        if (!found^ && current !== wip) {
          if (current.fiberTag == Types.Tags.Fiber.Host) {
            count := count^ + 1;
          } else {
            measure(current.child);
          }
          measure(current.sibling);
        } else {
          found := true;
        }
      };

      measure(parent.child);

      count^;
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
      Fiber.detach(oldFiber.alternate);
      oldFiber.alternate = None;
      newFiber.alternate = Some(oldFiber);
    };

    /**
     * Maps the new fiber to its parent's key map.
     */
    let mapFiberToParent = (parent: Fiber.t, newFiber: Fiber.t, index: int, key: option(string)) => {
      newFiber.parent = Some(parent);
      /**
       * Set fiber references
       */
      newFiber.index = index;
      newFiber.key = key;

      /**
       * Map current fiber to parent using the key or the index.
       */
      switch (key) {
        | Some(actualKey) => Opaque.Map.set(parent.map, actualKey, newFiber);
        | None => Opaque.Map.set(parent.map, index, newFiber);
      }
    };

    let replaceFiber = (parent: Fiber.t, oldFiber: Fiber.t, element: Types.Element.t, index: int, key: option(string)): Fiber.t => {
      let replacementFiber = Fiber.make(element.name, element.fiberTag, element.props);
      replacementFiber.constructor = element.constructor;
      replacementFiber.workTag = Types.Tags.Work.Replace;
      replacementFiber.workStringTag = "Replace";
      replacementFiber.identifier = element.name ++ "-" ++ string_of_int(Fiber.createIndex());

      attachFiberAlternate(oldFiber, replacementFiber);
      mapFiberToParent(parent, replacementFiber, index, key);
      
      replacementFiber;
    };

    let deleteFiber = (parent: Fiber.t, oldFiber: Fiber.t, index: int, key: option(string)): Fiber.t => {
      let deletionFiber = Fiber.make(oldFiber.name, oldFiber.fiberTag, oldFiber.props);
      deletionFiber.constructor = oldFiber.constructor;
      deletionFiber.workTag = Types.Tags.Work.Delete;
      deletionFiber.workStringTag = "Delete";
      deletionFiber.instance = oldFiber.instance;

      attachFiberAlternate(oldFiber, deletionFiber);
      mapFiberToParent(parent, deletionFiber, index, key);
      
      deletionFiber;
    };

    /**
     * Updates fiber from the given element.
     */
    let updateFiberFromElement = (parent: Fiber.t, oldFiber: Fiber.t, element: Types.Element.t, index: int, key: option(string)): Fiber.t => {
      let patchFiber = Fiber.make(oldFiber.name, oldFiber.fiberTag, element.props);

      /**
       * Set other fields
       */
      patchFiber.workTag = Types.Tags.Work.Update;
      patchFiber.workStringTag = "Update";
      patchFiber.constructor = oldFiber.constructor;
      patchFiber.instance = oldFiber.instance;
      patchFiber.identifier = oldFiber.identifier;

      attachFiberAlternate(oldFiber, patchFiber);
      mapFiberToParent(parent, patchFiber, index, key);

      patchFiber;
    };

    /**
     * Create fiber from element
     */
    let createFiberFromElement = (parent: Fiber.t, element: Types.Element.t, index: int, key: option(string)): Fiber.t => {
      let creationFiber = Fiber.make(element.name, element.fiberTag, element.props);

      /**
       * Set other fields
       */
      creationFiber.workTag = Types.Tags.Work.Placement;
      creationFiber.workStringTag = "Placement";
      creationFiber.constructor = element.constructor;
      creationFiber.identifier = element.name ++ "-" ++ string_of_int(Fiber.createIndex());

      mapFiberToParent(parent, creationFiber, index, key);

      creationFiber;
    };

    /**
     * Produces a new fiber that represents the new unit of work for the new tree.
     */
    let updateFiber = (parent: Fiber.t, oldFiber: option(Fiber.t), element: option(Types.Element.t), index: int, key: option(string)): option(Fiber.t) => {
      if (oldFiber != None) {
        let%Option actualOldFiber = oldFiber;

        if (actualOldFiber.workTag == Types.Tags.Work.Delete && element != None) {
          let%Option actualElement = element;
          Some(createFiberFromElement(parent, actualElement, index, key));
        } else if (element == None) {
          Some(deleteFiber(parent, actualOldFiber, index, key));
        } else {
          let%Option actualElement = element;
          if (actualElement.fiberTag != actualOldFiber.fiberTag) {
            Some(deleteFiber(parent, actualOldFiber, index, key));
          } else if (actualElement.constructor != actualOldFiber.constructor) {
            Some(deleteFiber(parent, actualOldFiber, index, key));
          } else {
            Some(updateFiberFromElement(parent, actualOldFiber, actualElement, index, key));
          }
        }
      } else {
        let%Option actualElement = element;
        Some(createFiberFromElement(parent, actualElement, index, key));
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

    let call = (current: option(Fiber.t), wip: Fiber.t, children: Types.Children.t): option(Fiber.t) => {
      let previousFiber: ref(option(Fiber.t)) = ref(None);

      /**
       * Connects the newest fiber to the wip tree
       */
      let linkFiber = (newFiber: option(Fiber.t), hasElement: bool): unit => {
        if (wip.child == None) {
          wip.child = newFiber;
        } else if (hasElement) {
          let%OptionUnit prev = previousFiber^;
          prev.sibling = newFiber;
        }

        previousFiber := newFiber;
      };

      let marked = Opaque.Set.make();

      /**
       * Perform work to new children
       */
      ignore({
        children |> Array.iteri((index, element: option(Types.Element.t)) => {
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
          let%OptionUnit iteratedFiber = oldFiber;
          if (!Opaque.Set.has(marked, iteratedFiber)) {
            linkFiber(Some(deleteFiber(wip, iteratedFiber, iteratedFiber.index, iteratedFiber.key)), false);
          }
          iterateFibers(iteratedFiber.sibling);
        };

        iterateFibers(actualCurrent.child);
      })

      wip.child;
    };
  };

  module Hooks = {
    /**
     * Represents a hook slot
     */
    module Slot = {
      type t = {
        tag: Types.Tags.Hook.t,
        mutable value: option(Opaque.t),
      };
    };

    /**
     * Setups the global context for hooks
     */
    module RenderContext = {
      let hookFiber: ref(option(Fiber.t)) = ref(None);
      let hookCursor: ref(int) = ref(0);

      let render = (current: option(Fiber.t), wip: Fiber.t) => {
        /**
         * Set rendering fiber
         */
        hookFiber := Some(wip);
        hookCursor := 0;

        /**
         * Assign hooks to the adjacent fiber
         */
        let%OptionUnit actualCurrent = current;
        wip.hooks = actualCurrent.hooks;
        actualCurrent.hooks = Opaque.Array.make();
      };

      let finishRender = () => {
        hookFiber := None;
        hookCursor := 0;
      };

      let getCurrentFiber = (): Fiber.t => {
        let%OptionOrError currentFiber = (hookFiber^, Exceptions.OutOfContextHookCall);
        currentFiber;
      };

      let make = (tag: Types.Tags.Hook.t): Slot.t => {
        /**
         * Get currently rendering fiber
         */
        let currentFiber = getCurrentFiber();
        /**
         * Allocate a hook slot
         */
        let current = hookCursor^;
        let slot: option(Slot.t) = Opaque.Array.get(currentFiber.hooks, current);
        hookCursor := current + 1;

        switch (slot) {
          /**
           * Return previously created slot
           */
          | Some(actualSlot) => {
            /**
             * Slot may be incompatible than the requested.
             */
            if (actualSlot.tag != tag) {
              raise(Exceptions.IncompatibleHook);
            }
            actualSlot;
          };
          | None => {
            /**
             * Create a slot instance
             */
            let newSlot: Slot.t = {
              tag: tag,
              value: None,
            };
            /**
             * Set the new slot on the allocated slot
             */
            Opaque.Array.set(currentFiber.hooks, current, newSlot);

            newSlot;
          };
        }
      };

      let forEach = (wip: Fiber.t, handler: Slot.t => unit): unit => {
        Opaque.Array.forEach(wip.hooks, (slot, _) => {
          handler(slot);
        });
      };
    };

    module Functions = {
      /**
       * Memoizes a callback reference.
       * 
       * Updates the reference if the dependency changes.
       */
      module Callback = {
        type t('i, 'o) = 'i => 'o;

        let use = (callback: t('i, 'o), dependency: 'dependency): t('i, 'o) => {
          let state = RenderContext.make(Types.Tags.Hook.Callback);
          let dep = RenderContext.make(Types.Tags.Hook.Dependency);

          switch (state.value) {
            | Some(actualCallback) => {
              if (dep.value != Some(Opaque.convert(dependency))) {
                state.value = Some(Opaque.convert(callback));
                dep.value = Some(Opaque.convert(dependency));
                callback;
              } else {
                Opaque.return(actualCallback);
              }
            };
            | None => {
              state.value = Some(Opaque.convert(callback));
              dep.value = Some(Opaque.convert(dependency));
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
          let state = RenderContext.make(Types.Tags.Hook.Constant);

          switch (state.value) {
            | Some(value) => Opaque.return(value);
            | None => {
              let value = supplier();
              state.value = Some(Opaque.convert(value));
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
          let wip = RenderContext.getCurrentFiber();

          let instance = Context.read(wip, context);

          Opaque.Set.add(wip.dependencies, context);

          instance.value;
        };
      };

      /**
       * Attaches a side-effect to the component that runs
       * after mutations and every time the dependency changes.
       */
      module Effect = {
        type cleanup = unit => unit;
        type effect = unit => option(cleanup);
        type slot = (
          effect,
          ref(option(cleanup)),
          Types.Tags.Work.t,
        );

        let use = (effect: effect, dependency: 'dependency): unit => {
          /**
           * Create hooks for effect and dep
           */
          let state = RenderContext.make(Types.Tags.Hook.Effect);
          let dep = RenderContext.make(Types.Tags.Hook.Dependency);

          switch (dep.value) {
            | Some(actualDep) => {
              /**
               * Compare dependency
               */
              if (Opaque.return(actualDep) != dependency) {
                /**
                 * Get previous slot value
                 */
                let%OptionUnit opaquePrev = state.value;
                let (effect, cleanup, _) = Opaque.return(opaquePrev);
                /**
                 * Update slot value
                 */
                state.value = Some(Opaque.convert((
                  effect,
                  cleanup,
                  Types.Tags.Work.Update,
                )));
                /**
                 * Update dependency value
                 */
                dep.value = Some(Opaque.convert(dependency));
              }
            };
            | None => {
              state.value = Some(Opaque.convert((
                effect,
                ref(None),
                Types.Tags.Work.Placement,
              )));
              dep.value = Some(Opaque.convert(dependency));
            };
          }
        };
      };

      /**
       * Creates a stable function that allows force updates.
       */
      module ForceUpdate = {
        let use = () => {
          let dispatch = RenderContext.make(Types.Tags.Hook.ForceUpdate);

          switch (dispatch.value) {
            | Some(actualValue) => Opaque.return(actualValue);
            | None => {
              let callback = () => {
                Core.updateScheduled := true;
              };

              dispatch.value = Some(Opaque.convert(callback));

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


        type slot = (
          effect,
          ref(option(cleanup)),
          Types.Tags.Work.t,
        );

        let use = (effect: effect, dependency: 'dependency): unit => {
          /**
           * Create hooks for effect and dep
           */
          let state = RenderContext.make(Types.Tags.Hook.LayoutEffect);
          let dep = RenderContext.make(Types.Tags.Hook.Dependency);

          switch (dep.value) {
            | Some(actualDep) => {
              /**
               * Compare dependency
               */
              if (Opaque.return(actualDep) != dependency) {
                /**
                 * Get previous slot value
                 */
                let%OptionUnit opaquePrev = state.value;
                let (effect, cleanup, _) = Opaque.return(opaquePrev);
                /**
                 * Update slot value
                 */
                state.value = Some(Opaque.convert((
                  effect,
                  cleanup,
                  Types.Tags.Work.Update,
                )));
                /**
                 * Update dependency value
                 */
                dep.value = Some(Opaque.convert(dependency));
              }
            };
            | None => {
              state.value = Some(Opaque.convert((
                effect,
                ref(None),
                Types.Tags.Work.Placement,
              )));
              dep.value = Some(Opaque.convert(dependency));
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
          let state = RenderContext.make(Types.Tags.Hook.Memo);
          let dep = RenderContext.make(Types.Tags.Hook.Dependency);

          switch (state.value) {
            | Some(actualState) => {
              if (dep.value != Some(Opaque.convert(dependency))) {
                let value = supplier();
                state.value = Some(Opaque.convert(value));
                dep.value = Some(Opaque.convert(dependency));
                value;
              } else {
                Opaque.return(actualState);
              }
            };
            | None => {
              let value = supplier();
              state.value = Some(Opaque.convert(value));
              dep.value = Some(Opaque.convert(dependency));
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
          let wip = RenderContext.getCurrentFiber();
          let state = RenderContext.make(Types.Tags.Hook.ReducerState);
          let dispatch = RenderContext.make(Types.Tags.Hook.ReducerDispatch);

          switch (state.value, dispatch.value) {
            | (Some(actualState), Some(actualDispatch)) => {
              (Opaque.return(actualState), Opaque.return(actualDispatch));
            };
            | (_, _) => {
              let initialState = initial();
              let callback = (action: 'action) => {
                let%OptionUnit opaqueValue = state.value;
                let actualValue = Opaque.return(opaqueValue);
                let newState = reducer(actualValue, action);

                if (newState != actualValue) {
                  state.value = Some(Opaque.convert(newState));
                  wip.shouldUpdate = true;
                  Core.updateScheduled := true;
                }
              };

              state.value = Some(Opaque.convert(initialState));
              dispatch.value = Some(Opaque.convert(callback));

              (initialState, callback);
            };
          }
        };
      };

      /**
       * Creates a reducer-based state
       */
      module State = {
        type action('state) = 'state => 'state;
        type tuple('state) = ('state, action('state) => unit);

        let use = (initial: unit => 'state): tuple('state) => {
          let wip = RenderContext.getCurrentFiber();
          let state = RenderContext.make(Types.Tags.Hook.State);
          let dispatch = RenderContext.make(Types.Tags.Hook.SetState);

          switch (state.value, dispatch.value) {
            | (Some(actualState), Some(actualDispatch)) => {
              (Opaque.return(actualState), Opaque.return(actualDispatch));
            };
            | (_, _) => {
              let initialState = initial();
              let callback = (action: action('state)) => {
                let%OptionUnit opaqueValue = state.value;
                let actualValue: 'state = Opaque.return(opaqueValue);
                let newState = action(actualValue);

                if (newState != actualValue) {
                  state.value = Some(Opaque.convert(newState));
                  wip.shouldUpdate = true;
                  Core.updateScheduled := true;
                }
              };

              state.value = Some(Opaque.convert(initialState));
              dispatch.value = Some(Opaque.convert(callback));

              (initialState, callback);
            };
          }
        };
      };

      /**
       * Creates a Reference instance.
       */
      module Reference = {
        let use = (initialValue: 'a): Types.Reference.t('a) => {
          let state = RenderContext.make(Types.Tags.Hook.Reference);

          switch (state.value) {
            | Some(value) => Opaque.return(value);
            | None => {
              let value = Types.Reference.Mutable(ref(Some(initialValue)));
              state.value = Some(Opaque.convert(value));
              value;
            };
          }
        };
      };

      /**
       * Creates a Mutable
       */
      module Mutable = {
        let use = (initialValue: 'a): ref('a) => {
          let state = RenderContext.make(Types.Tags.Hook.Mutable);

          switch (state.value) {
            | Some(value) => Opaque.return(value);
            | None => {
              let value = ref(initialValue);
              state.value = Some(Opaque.convert(value));
              value;
            };
          }
        };
      };

      module Identifier = {
        let use = (): string => {
          let wip = RenderContext.getCurrentFiber();
          let state = RenderContext.make(Types.Tags.Hook.Identifier);

          switch (state.value) {
            | Some(value) => Opaque.return(value);
            | None => {
              let value = wip.identifier;
              state.value = Some(Opaque.convert(value));
              value;
            };
          }
        };
      };
    };
  };

  module BeginWork = {
    let safelyRender = (wip: Fiber.t, render: unit => option(Types.Element.t)): option(Types.Element.t) => {
      switch (render()) {
        | result => result;
        | exception e => {
          wip.error = Some(Exceptions.Component(e, [| |]));
          None;
        };
      }
    };

    let updateBasic = (current: option(Fiber.t), wip: Fiber.t): option(Fiber.t) => {
      let result: option(Types.Element.t) = safelyRender(wip, () => {
        let%OptionOrError constructor = (wip.constructor, Exceptions.MissingBasicComponentConstructor);

        let render: Types.Component.t('props) = Opaque.return(constructor);
        let props: 'props = Opaque.return(wip.props);
        render({
          key: wip.key,
          ref: wip.ref,
        }, props);
      });

      if (result != None) {
        ReconcileChildren.call(current, wip, [| result |]);
      } else {
        None;
      }
    };

    let updateComponent = (current: option(Fiber.t), wip: Fiber.t): option(Fiber.t) => {
      Hooks.RenderContext.render(current, wip);
      let result: option(Types.Element.t) = safelyRender(wip, () => {
        let%OptionOrError constructor = (wip.constructor, Exceptions.MissingBasicComponentConstructor);

        let render: Types.Component.t('props) = Opaque.return(constructor);
        let props: 'props = Opaque.return(wip.props);
        render({
          key: wip.key,
          ref: wip.ref,
        }, props);
      });
      Hooks.RenderContext.finishRender();

      if (result != None) {
        ReconcileChildren.call(current, wip, [| result |]);
      } else {
        None;
      }
    };

    let updateContextConsumer = (current: option(Fiber.t), wip: Fiber.t): option(Fiber.t) => {
      let props: Context.Consumer.props('a) = Opaque.return(wip.props);
      
      let result: option(Types.Element.t) = safelyRender(wip, () => {
        let instance = Context.read(wip, props.context);

        let children: option(Types.Element.t) = if (instance.shouldUpdate) {
          props.build(instance.value);
        } else {
          let%Option actualCurrent = current;
          let result = actualCurrent.children;
          actualCurrent.children = None;
          result;
        };

        wip.children = children;

        children;
      });

      if (result != None) {
        ReconcileChildren.call(current, wip, [| result |]);
      } else {
        None;
      }
    };

    let updateContextProvider = (current: option(Fiber.t), wip: Fiber.t): option(Fiber.t) => {
      let props: Context.Provider.props('a) = Opaque.return(wip.props);
      let%OptionOrError actualValue = (props.value, Exceptions.DesyncContextValue);

      switch (wip.instance) {
        | None => {
          let instance: Context.Instance.t('a) = {
            value: actualValue,
            shouldUpdate: true,
          };
          wip.instance = Some(Opaque.convert(instance));
        };
        | Some(actualInstance) => {
          let instance: Context.Instance.t('a) = Opaque.return(actualInstance);
          let%OptionOrError actualCurrent = (current, Exceptions.UnboundContextInstance);
          let%OptionOrError prevOpaqueInstance = (actualCurrent.instance, Exceptions.UnboundContextInstance);
          let prevInstance: Context.Instance.t('a) = Opaque.return(prevOpaqueInstance);
          instance.shouldUpdate = actualValue != prevInstance.value;
          instance.value = actualValue;
        };
      }

      ReconcileChildren.call(current, wip, props.children);
    };

    let updateErrorBoundary = (current: option(Fiber.t), wip: Fiber.t): option(Fiber.t) => {
      let props: ErrorBoundary.props = Opaque.return(wip.props);

      ReconcileChildren.call(current, wip, props.children);
    };

    let updateFragment = (current: option(Fiber.t), wip: Fiber.t): option(Fiber.t) => {
      let props: Fragment.props = Opaque.return(wip.props);

      ReconcileChildren.call(current, wip, props.children);
    };

    let updateHost = (current: option(Fiber.t), wip: Fiber.t): option(Fiber.t) => {
      let props: Host.props = Opaque.return(wip.props);
      if (wip.instance == None) {
        let%OptionOrError constructor = (wip.constructor, Exceptions.InvalidHostConstructor);
        let stringConstructor: string = Opaque.return(constructor);
        let instance = Reconciler.createInstance(
          stringConstructor,
          props.attributes,
          wip.identifier,
          wip,
        );
        wip.instance = Some(Opaque.convert(instance));
      }
      ReconcileChildren.call(current, wip, props.children);
    };

    let updateMemoInitial = (current: option(Fiber.t), wip: Fiber.t): option(Fiber.t) => {
      Hooks.RenderContext.render(current, wip);
      let result: option(Types.Element.t) = safelyRender(wip, () => {
        let%OptionOrError constructor = (wip.constructor, Exceptions.MissingBasicComponentConstructor);

        let render: Types.Component.t('props) = Opaque.return(constructor);
        let props: 'props = Opaque.return(wip.props);
        render({
          key: wip.key,
          ref: wip.ref,
        }, props);
      });
      Hooks.RenderContext.finishRender();

      if (result != None) {
        wip.children = result;
        ReconcileChildren.call(current, wip, [| result |]);
      } else {
        None;
      }
    };

    let updateMemo = (current: option(Fiber.t), wip: Fiber.t): option(Fiber.t) => {
      switch (current) {
        | Some(actualCurrent) => {
          let shouldUpdate = ref(actualCurrent.shouldUpdate);

          if (!shouldUpdate^) {
            let deps = actualCurrent.dependencies;

            Opaque.Set.forEach(deps, (contextType) => {
              let instance = Context.read(wip, contextType);

              if (instance.shouldUpdate) {
                shouldUpdate := true;
              }
            });
          }

          let currentProps: 'propsA = Opaque.return(actualCurrent.props);
          let wipProps: 'propsB = Opaque.return(wip.props);

          if (shouldUpdate^ || (wipProps != currentProps)) {
            updateMemoInitial(current, wip);
          } else {
            let children = actualCurrent.children;
            wip.children = children;
            actualCurrent.children = None;
            ReconcileChildren.call(current, wip, [| children |]);
          }
        };
        | None => updateMemoInitial(current, wip);
      }
    };

    let updateMemoBasicInitial = (current: option(Fiber.t), wip: Fiber.t): option(Fiber.t) => {
      let result: option(Types.Element.t) = safelyRender(wip, () => {
        let%OptionOrError constructor = (wip.constructor, Exceptions.MissingBasicComponentConstructor);

        let render: Types.Component.t('props) = Opaque.return(constructor);
        let props: 'props = Opaque.return(wip.props);
        render({
          key: wip.key,
          ref: wip.ref,
        }, props);
      });

      if (result != None) {
        wip.children = result;
        ReconcileChildren.call(current, wip, [| result |]);
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
            let children = actualCurrent.children;
            wip.children = children;
            actualCurrent.children = None;
            ReconcileChildren.call(current, wip, [| children |]);
          }
        };
        | None => updateMemoInitial(current, wip);
      }
    };

    let updateRoot = (current: option(Fiber.t), wip: Fiber.t): option(Fiber.t) => {
      let props: Root.props = Opaque.return(wip.props);

      if (wip.instance == None) {
        wip.instance = Some(Opaque.convert(props.value));
      }

      ReconcileChildren.call(current, wip, [| props.children |]);
    };

    let call = (current: option(Fiber.t), wip: Fiber.t): option(Fiber.t) => {
      switch (wip.fiberTag) {
        | Types.Tags.Fiber.Basic => updateBasic(current, wip);
        | Types.Tags.Fiber.Component => updateComponent(current, wip);
        | Types.Tags.Fiber.ContextConsumer => updateContextConsumer(current, wip);
        | Types.Tags.Fiber.ContextProvider => updateContextProvider(current, wip);
        | Types.Tags.Fiber.ErrorBoundary => updateErrorBoundary(current, wip);
        | Types.Tags.Fiber.Fragment => updateFragment(current, wip);
        | Types.Tags.Fiber.Host => updateHost(current, wip);
        | Types.Tags.Fiber.Memo => updateMemo(current, wip);
        | Types.Tags.Fiber.MemoBasic => updateMemoBasic(current, wip);
        | Types.Tags.Fiber.Root => updateRoot(current, wip);
      }
    };
  }

  module Commit = {
    module Error = {
      let raiseToParent = (wip: Fiber.t, error: exn) => {
        let%OptionOrError parent = (wip.parent, error);
        parent.error = Some(error);
      };

      let call = (wip: Fiber.t) => {
        let%OptionUnit error = wip.error;
        if (wip.fiberTag == Types.Tags.Fiber.ErrorBoundary) {
          let props: ErrorBoundary.props = Opaque.return(wip.props);

          switch (error) {
            | Exceptions.Component(value, trace) => {
              try (props.onError(value, trace)) {
                | err => raiseToParent(wip, Exceptions.Component(err, trace));
              }
            };
            | value => {
              let trace = [| wip.name |];
              try (props.onError(value, trace)) {
                | err => raiseToParent(wip, Exceptions.Component(err, trace));
              }
            };
          }
        } else {
          switch (error) {
            | Exceptions.Component(value, trace) => {
              raiseToParent(wip, Exceptions.Component(value, Array.append(trace, [| wip.identifier |])));
            }
            | value => {
              raiseToParent(wip, Exceptions.Component(value, [| wip.identifier |]));
            };
          }
        }
      };
    };

    module Placement = {
      let withHost = (wip: Fiber.t): unit => {
        let%OptionUnit parent = Utils.getHostParent(wip);
        let%OptionUnit parentInstance = parent.instance;
        let%OptionUnit childInstance = wip.instance;

        Reconciler.appendChild(
          Opaque.return(parentInstance),
          Opaque.return(childInstance),
          Utils.getInstanceIndex(parent, wip),
          wip,
        );

        switch (wip.ref) {
          | Types.Reference.None => ();
          | Types.Reference.Mutable(pointer) => pointer := Some(Opaque.convert(childInstance));
          | Types.Reference.Callable(pointer) => pointer(Opaque.convert(childInstance));
        }
      };

      type tuple = Hooks.Functions.LayoutEffect.slot;

      let withHooks = (wip: Fiber.t): unit => {
        Opaque.Array.forEach(wip.hooks, (hook, _) => {
          let { tag, value }: Hooks.Slot.t = Opaque.return(hook);

          if (tag == Types.Tags.Hook.LayoutEffect) {
            let%OptionUnit actualValue = value;
            let (effect, cleanup, workTag): tuple = Opaque.return(actualValue);

            if (workTag == Types.Tags.Work.Placement) {
              cleanup := effect();
            }
          }
        });
      };

      let call = (wip: Fiber.t): unit => {
        switch (wip.fiberTag) {
          | Types.Tags.Fiber.Host => withHost(wip);
          | Types.Tags.Fiber.Component => withHooks(wip);
          | Types.Tags.Fiber.Memo => withHooks(wip);
          | _ => ();
        }
      };
    };

    module Update = {
      let withHost = (wip: Fiber.t): unit => {
        let%OptionUnit parent = Utils.getHostParent(wip);
        let%OptionUnit childInstance = wip.instance;
        let%OptionUnit alternate = wip.alternate;
        
        let returnedAltProps: Host.props = Opaque.return(alternate.props);
        let returnedProps: Host.props = Opaque.return(wip.props);

        Reconciler.commitUpdate(
          Opaque.return(childInstance),
          returnedAltProps.attributes,
          returnedProps.attributes,
          Utils.getInstanceIndex(parent, wip),
          wip,
        );
      };

      type tuple = Hooks.Functions.LayoutEffect.slot;

      let withHooks = (wip: Fiber.t): unit => {
        Opaque.Array.forEach(wip.hooks, (hook, _) => {
          let { tag, value }: Hooks.Slot.t = Opaque.return(hook);

          if (tag == Types.Tags.Hook.LayoutEffect) {
            let%OptionUnit actualValue = value;
            let (effect, cleanup, workTag): tuple = Opaque.return(actualValue);

            if (workTag == Types.Tags.Work.Update) {
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
          | Types.Tags.Fiber.Host => withHost(wip);
          | Types.Tags.Fiber.Component => withHooks(wip);
          | Types.Tags.Fiber.Memo => withHooks(wip);
          | _ => ();
        }
      };
    };

    module Delete = {
      let withHost = (wip: Fiber.t): unit => {
        let%OptionUnit parent = Utils.getHostParent(wip);
        let%OptionUnit parentInstance = parent.instance;
        let%OptionUnit childInstance = wip.instance;

        Reconciler.removeChild(
          Opaque.return(parentInstance),
          Opaque.return(childInstance),
          Utils.getInstanceIndex(parent, wip),
          wip,
        );

        switch (wip.ref) {
          | Types.Reference.Mutable(pointer) => pointer := None;
          | _ => ();
        }
      };

      type tuple = Hooks.Functions.LayoutEffect.slot;

      let withHooks = (wip: Fiber.t): unit => {
        Opaque.Array.forEach(wip.hooks, (hook, _) => {
          let { tag, value }: Hooks.Slot.t = Opaque.return(hook);

          if (tag == Types.Tags.Hook.LayoutEffect) {
            let%OptionUnit actualValue = value;
            let (_, cleanup, _): tuple = Opaque.return(actualValue);
            let%OptionUnit actualCleanup = cleanup^;
            actualCleanup();
          }
        });
      };

      let commitDelete = (wip: Fiber.t): unit => {
        switch (wip.fiberTag) {
          | Types.Tags.Fiber.Host => withHost(wip);
          | Types.Tags.Fiber.Component => withHooks(wip);
          | Types.Tags.Fiber.Memo => withHooks(wip);
          | _ => ();
        }
      };

      let rec commitDeleteSibling = (wip: option(Fiber.t)) => {
        let%OptionUnit actualFiber = wip;
        commitDelete(actualFiber);
        commitDeleteSibling(actualFiber.sibling);
      };

      let call = (wip: Fiber.t) => {
        commitDelete(wip);
        commitDeleteSibling(wip.child);
      };
    };

    module Lifecycles = {
      module Placement = {
        type tuple = Hooks.Functions.Effect.slot;

        let withHooks = (wip: Fiber.t): unit => {
          Opaque.Array.forEach(wip.hooks, (hook, _) => {
            let { tag, value }: Hooks.Slot.t = Opaque.return(hook);

            if (tag == Types.Tags.Hook.Effect) {
              let%OptionUnit actualValue = value;
              let (effect, cleanup, workTag): tuple = Opaque.return(actualValue);


              if (workTag == Types.Tags.Work.Placement) {
                cleanup := effect();
              }
            }
          });
        };

        let call = (wip: Fiber.t): unit => {
          switch (wip.fiberTag) {
            | Types.Tags.Fiber.Component => withHooks(wip);
            | Types.Tags.Fiber.Memo => withHooks(wip);
            | _ => ();
          }
        };
      };

      module Update = {
        type tuple = Hooks.Functions.Effect.slot;

        let withHooks = (wip: Fiber.t): unit => {
          Opaque.Array.forEach(wip.hooks, (hook, _) => {
            let { tag, value }: Hooks.Slot.t = Opaque.return(hook);

            if (tag == Types.Tags.Hook.Effect) {
              let%OptionUnit actualValue = value;
              let (effect, cleanup, workTag): tuple = Opaque.return(actualValue);

              if (workTag == Types.Tags.Work.Update) {
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
            | Types.Tags.Fiber.Component => withHooks(wip);
            | Types.Tags.Fiber.Memo => withHooks(wip);
            | _ => ();
          }
        };
      };

      module Delete = {
        type tuple = Hooks.Functions.Effect.slot;

        let withHooks = (wip: Fiber.t): unit => {
          Opaque.Array.forEach(wip.hooks, (hook, _) => {
            let { tag, value }: Hooks.Slot.t = Opaque.return(hook);

            if (tag == Types.Tags.Hook.Effect) {
              let%OptionUnit actualValue = value;
              let (_, cleanup, _): tuple = Opaque.return(actualValue);
              let%OptionUnit actualCleanup = cleanup^;
              actualCleanup();
            }
          });
        };

        let commitDelete = (wip: Fiber.t): unit => {
          switch (wip.fiberTag) {
            | Types.Tags.Fiber.Component => withHooks(wip);
            | Types.Tags.Fiber.Memo => withHooks(wip);
            | _ => ();
          }
        };

        let rec commitDeleteSibling = (wip: option(Fiber.t)) => {
          let%OptionUnit actualFiber = wip;
          commitDelete(actualFiber);
          commitDeleteSibling(actualFiber.sibling);
        };

        let call = (wip: Fiber.t) => {
          commitDelete(wip);
          commitDeleteSibling(wip.child);
        };
      };

      module Work = {
        let commitWork = (wip: Fiber.t, commit: Fiber.t => unit, alternate: option(Fiber.t)) => {
          switch (alternate) {
            | None => commit(wip);
            | Some(alt) => commit(alt);
          }
        };

        let rec call = (wip: option(Fiber.t)): unit => {
          let%OptionUnit commitingFiber = wip;
          let commitOnChild = ref(true);

          let commitSelf = () => {
            switch (commitingFiber.workTag) {
              | Types.Tags.Work.Placement => {
                commitWork(commitingFiber, Placement.call, None);
              };
              | Types.Tags.Work.Update => {
                commitWork(commitingFiber, Update.call, None);
              };
              | Types.Tags.Work.Delete => {
                commitWork(commitingFiber, Delete.call, commitingFiber.alternate);
                commitOnChild := false;
              };
              | Types.Tags.Work.Replace => {
                commitWork(commitingFiber, Delete.call, commitingFiber.alternate);
                commitWork(commitingFiber, Placement.call, None);
              };
              | _ => ();
            }
          }

          commitSelf();

          if (commitingFiber.error != None) {
            Error.call(commitingFiber);
          } else if (commitOnChild^) {
            call(commitingFiber.child);

            if (commitingFiber.error != None) {
              Error.call(commitingFiber);
            }
          }

          call(commitingFiber.sibling);
        };
      };
    };

    module Work = {
      let commitWork = (wip: Fiber.t, commit: Fiber.t => unit, alternate: option(Fiber.t)) => {
        switch (alternate) {
          | None => commit(wip);
          | Some(alt) => commit(alt);
        }
      };

      let rec call = (wip: option(Fiber.t)): unit => {
        let%OptionUnit commitingFiber = wip;
        let commitOnChild = ref(true);

        let commitSelf = () => {
          switch (commitingFiber.workTag) {
            | Types.Tags.Work.Placement => {
              commitWork(commitingFiber, Placement.call, None);
            };
            | Types.Tags.Work.Update => {
              commitWork(commitingFiber, Update.call, None);
            };
            | Types.Tags.Work.Delete => {
              commitWork(commitingFiber, Delete.call, commitingFiber.alternate);
              commitOnChild := false;
            };
            | Types.Tags.Work.Replace => {
              commitWork(commitingFiber, Delete.call, commitingFiber.alternate);
              commitWork(commitingFiber, Placement.call, None);
            };
            | _ => ();
          }
        }

        commitSelf();

        if (commitingFiber.error != None) {
          Error.call(commitingFiber);
        } else if (commitOnChild^) {
          call(commitingFiber.child);

          if (commitingFiber.error != None) {
            Error.call(commitingFiber);
          }
        }

        call(commitingFiber.sibling);
      };
    };

    module Root = {
      let call = () => {
        /**
         * Reset status for schedule update
         */
        Core.updateScheduled := false;

        /**
         * Make sure that we have a wip root
         */
        let%OptionOrError wip = (Core.root.wip, Exceptions.MissingWorkInProgressRoot);
        
        /**
         * Commit before mutations
         */
        Work.call(wip.child);
          /**
         * Detach the current root
         * and set the current root to the wip root
         */
        ignore({
          let%OptionUnit current = Core.root.current;
          Fiber.detach(current.alternate);
        });

        Core.root.current = Core.root.wip;
        Core.root.wip = None;

        /**
         * Commit after mutations
         */
        Lifecycles.Work.call(wip.child);

        if (Core.updateScheduled^) {
          Core.update();
          Core.updateScheduled := false;
        }
      }
    };
  };

  let rec completeUnitOfWork = (wip: option(Fiber.t)): option(Fiber.t) => {
    let%Option actualWIP = wip;
    let sibling = actualWIP.sibling;

    if (sibling == None) {
      completeUnitOfWork(actualWIP.parent);
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

  exception Test;

  let workLoop = (deadline: unit => float) => {
    let shouldYield = ref(false);

    let rec loop = (deadline: unit => float) => {
      if (!shouldYield^) {
        let%OptionUnit nextUnitOfWork = Core.root.next;

        Core.root.next = performUnitOfWork(
          nextUnitOfWork.alternate,
          nextUnitOfWork,
        );

        shouldYield := deadline() < 1.0;

        loop(deadline);
      }
    };

    loop(deadline);

    if (Core.root.next == None && Core.root.wip != None) {
      Commit.Root.call();
    }
  };

  /**
   * Hooks
   */
  let useCallback = Hooks.Functions.Callback.use;
  let useConstant = Hooks.Functions.Constant.use;
  let useContext = Hooks.Functions.Context.use;
  let useEffect = Hooks.Functions.Effect.use;
  let useForceUpdate = Hooks.Functions.ForceUpdate.use;
  let useIdentifier = Hooks.Functions.Identifier.use;
  let useLayoutEffect = Hooks.Functions.LayoutEffect.use;
  let useMemo = Hooks.Functions.Memo.use;
  let useMutable = Hooks.Functions.Mutable.use;
  let useReducer = Hooks.Functions.Reducer.use;
  let useReference = Hooks.Functions.Reference.use;
  let useState = Hooks.Functions.State.use;
};
