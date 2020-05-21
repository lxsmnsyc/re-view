
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
};

module Work = {
  type t =
    | None
    | Performed
    | Placement
    | Update
    | Delete
    | Replace
  ;
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
    | Ref
    | State
    | SetState
    | ReducerState
    | ReducerDispatch
  ;
};
