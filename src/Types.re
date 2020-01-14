module Node {
  type t;

}

module Renderer {
  module Result {
    type t =
      | None
      | Some(Node.t)
      | Fragment(list(t))
    ;
  };

  type t('a) = 'a => Result.t;
}

module Component {
  type t('a) = {
    name: string,
    render: Renderer.t('a),
  };
}

module RenderContext {
  type t = {
    node: Node.t,
    parent: Node.t,
    root: Node.t,
    slot: ref(int),
    index: int,
  };
}

module Reconciler {
  module Render {
    type t = (Node.t, Node.t, option(int), option(Node.t)) => unit;
  }
}

module EquatableNode {
  type t('a) = {
    component: Component.t('a),
    props: 'a,
  };
}

module State {
  module Effect {
    module Cleanup {
      type t = unit => unit;
    }

    type t = unit => option(Cleanup.t);
  }

  type t('state, 'action, 'dependency) =
    | Ref('state)
    | Constant('state)
    | BasicState('state)
    | SetState('state)
    | ReducerState('state)
    | ReducerDispatch('action)
    | Effect(Effect.t)
    | EffectCleanup(option(Effect.Cleanup.t))
    | Dependency('dependency)
  ;
}