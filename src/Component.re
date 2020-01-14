let make = (name: string, render: Types.Renderer.t('a)): Types.Renderer.t('a) => {
  let component: Types.Component.t('a) = {
    name,
    render,
  };

  (props) => {
    let node = Node.make(Some(component), props);

    Some(node);
  };
};
