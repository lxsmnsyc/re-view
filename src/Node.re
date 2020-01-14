
[@bs.module "./bindings/Node.js"]
[@bs.new]
external make: (option(Types.Component.t('a)), 'a) => Types.Node.t = "Node";

[@bs.get]
external props: Types.Node.t => option('a) = "props";

[@bs.get]
external nodes: Types.Node.t => array(Types.Node.t) = "nodes";

[@bs.get]
external parent: Types.Node.t => option(Types.Node.t) = "parent";

[@bs.get]
external component: Types.Node.t => option(Types.Component.t('a)) = "component";

[@bs.send]
external getNode: (Types.Node.t, int) => option(Types.Node.t) = "getNode";

[@bs.send]
external setNode: (Types.Node.t, int, Types.Node.t) => unit = "setNode";

[@bs.send]
external clearNode: (Types.Node.t, int) => unit = "clearNode";

[@bs.send]
external replicate: (Types.Node.t, Types.Node.t) => unit = "replicate";

[@bs.send]
external mount: (Types.Node.t, Types.Node.t) => unit = "mount";

[@bs.send]
external unmount: Types.Node.t => unit = "unmount";

[@bs.set]
external setMountSchedule: (Types.Node.t, bool) => unit = "mountSchedule";

[@bs.set]
external setUnmountSchedule: (Types.Node.t, bool) => unit = "unmountSchedule";

[@bs.get]
external getMountSchedule: Types.Node.t => bool = "mountSchedule";

[@bs.get]
external getUnmountSchedule: Types.Node.t => bool = "unmountSchedule";

[@bs.send]
external getState: (Types.Node.t, int) => option('a) = "getState";

[@bs.send]
external setState: (Types.Node.t, int, 'a) => unit = "setState";

[@bs.module "./bindings/Node.js"]
[@bs.val]
external toEquatable: Types.Node.t => Types.EquatableNode.t('a) = "toEquatable";