[@bs.module "./bindings/RenderContext.js"]
[@bs.val]
external setContext: Types.RenderContext.t => unit = "setContext";

[@bs.module "./bindings/RenderContext.js"]
[@bs.val]
external getContext: unit => Types.RenderContext.t = "getContext";

[@bs.module "./bindings/RenderContext.js"]
[@bs.val]
external popContext: unit => unit = "popContext";

[@bs.module "./bindings/RenderContext.js"]
[@bs.val]
external isActive: unit => bool = "isActive";

