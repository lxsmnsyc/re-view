module type ReconcilerType = {
  type t;

  let createInstance: (string, 'props) => t;
  let appendChild: (t, t, int) => unit;
  let removeChild: (t, t, int) => unit;
  let commitUpdate: (t, 'props, 'props, int) => unit;
};
