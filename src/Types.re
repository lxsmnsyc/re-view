module Node {
  type t;
}

module Renderer {
  type t('a) = 'a => option(Node.t);
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