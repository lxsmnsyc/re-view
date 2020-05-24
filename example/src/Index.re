// Entry point
[@bs.val] external document: Js.t({..}) = "document";

open ReView;

let initialImage = "https://images.dog.ceo/breeds/frise-bichon/jh-ezio-3.jpg";
let finalImage = "https://images.dog.ceo/breeds/spaniel-cocker/n02102318_2984.jpg";

let imageContext = DOM.Context.make("ImageContext", initialImage);
let testContext = DOM.Context.make("TestContext", initialImage);

exception Test;

module Image = DOM.Component({
  type props = unit;

  let name = "Image";

  let make = (_, _) => {
    let state = DOM.useContext(imageContext);

    DOM.Element.make("img", { key: None, ref: None}, {
      attributes: DOM.Element.attributes(
        ~src=state,
        (),
      ),
      children: [||],
    });
  };
});

module ImageSrc = DOM.Component({
  type props = unit;

  let name = "ImageSrc";

  let make = (_, _) => {
    let state = DOM.useContext(imageContext);

    DOM.Text.make("Current source: " ++ state);
  };
});

module ImageGroup = DOM.Component({
  type props = unit;

  let name = "ImageGroup";

  let make = (_, _) => {
    DOM.Element.make("div", { key: None, ref: None }, {
      attributes: DOM.Element.attributes(),
      children: [|
        ImageSrc.make({ key: None, ref: None }, ()),
        Image.make({ key: None, ref: None }, ()),
        Image.make({ key: None, ref: None }, ()),
      |],
    });
  };
});

module Content = DOM.Component({
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
      setState(_ => finalImage);

      None;
    }, state);

    let (context, setContext) = DOM.useState(() => testContext);

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
