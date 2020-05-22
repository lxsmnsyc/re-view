// Entry point

[@bs.val] external document: Js.t({..}) = "document";

open ReView;

let initialImage = "https://images.dog.ceo/breeds/frise-bichon/jh-ezio-3.jpg";
let finalImage = "https://images.dog.ceo/breeds/spaniel-cocker/n02102318_2984.jpg";
let state = initialImage;

module App = DOM.Component({
  type props = unit;

  let make = (_, _) => {
    let (state, setState) = DOM.useState(() => initialImage);

    DOM.useEffect(() => {
      setState(_ => finalImage);

      None;
    }, state);

    DOM.Element.make("div", { key: None, ref: None }, {
      attributes: DOM.Element.attributes(),
      children: Some([|
        DOM.Element.make("img", { key: None, ref: None}, {
          attributes: DOM.Element.attributes(
            ~src=state,
            (),
          ),
          children: None,
        }),
        DOM.Element.make("img", { key: None, ref: None}, {
          attributes: DOM.Element.attributes(
            ~src=state,
            (),
          ),
          children: None,
        }),
      |]),
    });
  };
});

DOM.render(
  App.make({ key: None, ref: None}, ()),
  document##body,
);
