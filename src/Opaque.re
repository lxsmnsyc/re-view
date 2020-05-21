type t;

[@bs.module]
external return: t => 'a = "./bindings/identity.js";

[@bs.module]
external convert: 'a => t = "./bindings/identity.js";

module Array = {
  type t;

  [@bs.module "./bindings/native-array.js"]
  external make: unit => t = "makeArray";

  [@bs.module "./bindings/native-array.js"]
  external set: (t, int, 'value) => unit = "arraySet";

  [@bs.module "./bindings/native-array.js"]
  external get: (t, int) => option('value) = "arrayGet";

  [@bs.module "./bindings/native-array.js"]
  external forEach: (t, ('value, int) => unit) => unit = "arrayForEach";
};

module Map = {
  type t;

  [@bs.module "./bindings/native-map.js"]
  external make: unit => t = "makeMap";

  [@bs.module "./bindings/native-map.js"]
  external set: (t, 'key, 'value) => unit = "mapSet";

  [@bs.module "./bindings/native-map.js"]
  external get: (t, 'key) => option('value) = "mapGet";

  [@bs.module "./bindings/native-map.js"]
  external has: (t, 'key) => bool = "mapHas";
};

module Set = {
  type t;

  [@bs.module "./bindings/native-set.js"]
  external make: unit => t = "makeSet";

  [@bs.module "./bindings/native-set.js"]
  external add: (t, 'value) => unit = "setAdd";

  [@bs.module "./bindings/native-set.js"]
  external has: (t, 'value) => bool = "setHas";

  [@bs.module "./bindings/native-set.js"]
  external forEach: (t, ('value) => unit) => unit = "setForEach";
};