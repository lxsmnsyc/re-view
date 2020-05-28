// Entry point
[@bs.val] external document: Js.t({..}) = "document";
[@bs.val] external setTimeout: (unit => unit, int) => unit = "setTimeout";

open ReView;

let initialImage = "https://images.dog.ceo/breeds/frise-bichon/jh-ezio-3.jpg";
let finalImage = "https://images.dog.ceo/breeds/spaniel-cocker/n02102318_2984.jpg";

let imageContext = DOM.Context.make("ImageContext", initialImage);
let testContext = DOM.Context.make("TestContext", initialImage);

exception Test;

module Image = DOM.Memo({
  type props = unit;

  let name = "Image";

  let make = (_, _) => {
    let state = DOM.useContext(imageContext);

    DOM.Element.make({ key: None, ref: None}, {
      tag: "img",
      attributes: DOM.Element.attributes(
        ~src=state,
        (),
      ),
      children: [||],
    });
  };
});

module ImageSrc = DOM.Memo({
  type props = unit;

  let name = "ImageSrc";

  let make = (_, _) => {
    let state = DOM.useContext(imageContext);

    if (state == initialImage) {
      DOM.Fragment.make({ key: None, ref: None}, {
        children: [|
          DOM.Text.make("Current source: " ++ state),
          DOM.Text.make("Current source: " ++ state),
        |],
      });
    } else {
      DOM.Fragment.make({ key: None, ref: None}, {
        children: [|
          None,
          DOM.Text.make("Current source: " ++ state),
        |],
      });
    }
  };
});

module ImageGroup = DOM.Memo({
  type props = unit;

  let name = "ImageGroup";

  let make = (_, _) => {
    DOM.Element.make({ key: None, ref: None }, {
      tag: "div",
      attributes: DOM.Element.attributes(),
      children: [|
        ImageSrc.make({ key: None, ref: None }, ()),
        Image.make({ key: None, ref: None }, ()),
        Image.make({ key: None, ref: None }, ()),
      |],
    });
  };
});

module Content = DOM.Memo({
  type props = unit;

  let name = "Content";

  let make = (_, _) => {
    DOM.Fragment.make({ key: None, ref: None }, {
      children: [|
        ImageGroup.make({ key: None, ref: None }, ()),
        ImageGroup.make({ key: None, ref: None }, ()),
      |],
    });
  };
});

module App = DOM.Component({
  type props = unit;

  let name = "App";

  let make = (_, _) => {
    let (state, setState) = DOM.useState(() => initialImage);

    DOM.useLayoutEffect(() => {
      setTimeout(() => {
        setState(_ => finalImage);
      }, 5000);

      None;
    }, state);

    let (context, setContext) = DOM.useState(() => testContext);

    Js.log(state);

    DOM.ErrorBoundary.make({ key: None, ref: None }, {
      onError: (value, trace) => {
        Js.log(value);
        Js.log(trace);

        setContext(_ => imageContext);
      },
      children: [|
        DOM.Context.Provider.make({ key: None, ref: None }, {
          context: context,
          value: Some(state),
          children: [|
            Content.make({ key: None, ref: None }, ()),
          |],
        }),
      |],
    })

  };
});

DOM.render(
  App.make({ key: None, ref: None}, ()),
  document##body,
);
