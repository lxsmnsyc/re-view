type t = {
  constructor: option(Opaque.t),
  fiberTag: Tags.Fiber.t,
  key: option(string),
  ref: option(Reference.t(Opaque.t)),
  props: Opaque.t,
};

type base = {
  key: option(string),
  ref: option(Reference.t(Opaque.t)),
};
