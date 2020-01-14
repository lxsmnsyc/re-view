[@bs.module "./bindings/ModuleResolver.js"]
[@bs.val]
external getModule: string => 'a = "getModule";

[@bs.module "./bindings/ModuleResolver.js"]
[@bs.val]
external setModule: (string, 'a) => unit = "setModule";

