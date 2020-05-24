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
module Tags = {
  module Fiber = {
    type t =
      | Component
      | Host
      | Root
      | Fragment
      | Basic
      | Memo
      | MemoBasic
      | ErrorBoundary
      | ContextProvider
      | ContextConsumer
    ;

    let map = (tag: t) => {
      switch (tag) {
        | Component => "Component"
        | Host => "Host"
        | Root => "Root"
        | Fragment => "Fragment"
        | Basic => "Basic"
        | Memo => "Memo"
        | MemoBasic => "MemoBasic"
        | ErrorBoundary => "ErrorBoundary"
        | ContextProvider => "ContextProvider"
        | ContextConsumer => "ContextConsumer"
      }
    };
  };

  module Work = {
    type t =
      | None
      | Placement
      | Update
      | Delete
      | Replace
    ;

    let map = (tag: t) => {
      switch (tag) {
        | None => "None"
        | Placement => "Placement"
        | Update => "Update"
        | Delete => "Delete"
        | Replace => "Replace"
      }
    };
  };

  module Hook = {
    type t =
      | Callback
      | Constant
      | Context
      | Dependency
      | Effect
      | ForceUpdate
      | LayoutEffect
      | Memo
      | State
      | SetState
      | ReducerState
      | ReducerDispatch
      | Mutable
      | Reference
      | Identifier
    ;
  };
};

module Reference = {
  type t('a) =
    | None
    | Mutable(ref(option('a)))
    | Callable('a => unit)
  ;
};

module Element = {
  type t = {
    name: string,
    constructor: option(Opaque.t),
    fiberTag: Tags.Fiber.t,
    key: option(string),
    ref: Reference.t(Opaque.t),
    props: Opaque.t,
  };

  type base = {
    key: option(string),
    ref: Reference.t(Opaque.t),
  };
};

module Component = {
  type t('props) = (Element.base, 'props) => option(Element.t);
};

module type Component = {
  type props;

  let make: Component.t(props);

  let name: string;
};

module Children = {
  type t = array(option(Element.t));
};
