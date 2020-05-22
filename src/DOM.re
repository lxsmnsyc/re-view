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
module Reconciler: Reconciler.ReconcilerType = {
  type t;

  [@bs.module "./bindings/dom.js"]
  external createInstance: (string, 'props) => t = "createInstance";
  [@bs.module "./bindings/dom.js"]
  external appendChild: (t, t, int) => unit = "appendChild";
  [@bs.module "./bindings/dom.js"]
  external removeChild: (t, t, int) => unit = "removeChild";
  [@bs.module "./bindings/dom.js"]
  external commitUpdate: (t, 'props, 'props, int) => unit = "removeChild";
};

module DomCore = Engine.Make(Reconciler);

let render = DomCore.Core.render;

/**
 * Hooks
 */
let useCallback = DomCore.useCallback;
let useConstant = DomCore.useConstant;
let useContext = DomCore.useContext;
let useEffect = DomCore.useEffect;
let useForceUpdate = DomCore.useForceUpdate;
let useIdentifier = DomCore.useIdentifier;
let useLayoutEffect = DomCore.useLayoutEffect;
let useMemo = DomCore.useMemo;
let useMutable = DomCore.useMutable;
let useReducer = DomCore.useReducer;
let useReference = DomCore.useReference;
let useState = DomCore.useState;

/**
 * Components
 */
module Context = DomCore.Context;
module Fragment = DomCore.Fragment;
module ErrorBoundary = DomCore.ErrorBoundary;
module Basic = DomCore.Basic;
module MemoBasic = DomCore.MemoBasic;
module Component = DomCore.Component;
module Memo = DomCore.Memo;
