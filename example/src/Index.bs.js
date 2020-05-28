'use strict';

var Curry = require("bs-platform/lib/js/curry.js");
var DOM$ReView = require("re-view/src/DOM.bs.js");
var Caml_exceptions = require("bs-platform/lib/js/caml_exceptions.js");

var initialImage = "https://images.dog.ceo/breeds/frise-bichon/jh-ezio-3.jpg";

var finalImage = "https://images.dog.ceo/breeds/spaniel-cocker/n02102318_2984.jpg";

var imageContext = Curry._2(DOM$ReView.DomCore.Context.make, "ImageContext", initialImage);

var testContext = Curry._2(DOM$ReView.DomCore.Context.make, "TestContext", initialImage);

var Test = Caml_exceptions.create("Index-ReasonReactExamples.Test");

function make(param, param$1) {
  var state = Curry._1(DOM$ReView.useContext, imageContext);
  return DOM$ReView.$$Element.make({
              key: undefined,
              ref: undefined
            }, {
              tag: "img",
              attributes: {
                src: state
              },
              children: []
            });
}

var $$Image = Curry._1(DOM$ReView.DomCore.Memo, {
      make: make,
      name: "Image"
    });

function make$1(param, param$1) {
  var state = Curry._1(DOM$ReView.useContext, imageContext);
  if (state === initialImage) {
    return Curry._2(DOM$ReView.DomCore.Fragment.make, {
                key: undefined,
                ref: undefined
              }, {
                children: [
                  DOM$ReView.$$Text.make("Current source: " + state),
                  DOM$ReView.$$Text.make("Current source: " + state)
                ]
              });
  } else {
    return Curry._2(DOM$ReView.DomCore.Fragment.make, {
                key: undefined,
                ref: undefined
              }, {
                children: [
                  undefined,
                  DOM$ReView.$$Text.make("Current source: " + state)
                ]
              });
  }
}

var ImageSrc = Curry._1(DOM$ReView.DomCore.Memo, {
      make: make$1,
      name: "ImageSrc"
    });

function make$2(param, param$1) {
  return DOM$ReView.$$Element.make({
              key: undefined,
              ref: undefined
            }, {
              tag: "div",
              attributes: { },
              children: [
                Curry._2(ImageSrc.make, {
                      key: undefined,
                      ref: undefined
                    }, undefined),
                Curry._2($$Image.make, {
                      key: undefined,
                      ref: undefined
                    }, undefined),
                Curry._2($$Image.make, {
                      key: undefined,
                      ref: undefined
                    }, undefined)
              ]
            });
}

var ImageGroup = Curry._1(DOM$ReView.DomCore.Memo, {
      make: make$2,
      name: "ImageGroup"
    });

function make$3(param, param$1) {
  return Curry._2(DOM$ReView.DomCore.Fragment.make, {
              key: undefined,
              ref: undefined
            }, {
              children: [
                Curry._2(ImageGroup.make, {
                      key: undefined,
                      ref: undefined
                    }, undefined),
                Curry._2(ImageGroup.make, {
                      key: undefined,
                      ref: undefined
                    }, undefined)
              ]
            });
}

var Content = Curry._1(DOM$ReView.DomCore.Memo, {
      make: make$3,
      name: "Content"
    });

function make$4(param, param$1) {
  var match = Curry._1(DOM$ReView.useState, (function (param) {
          return initialImage;
        }));
  var setState = match[1];
  var state = match[0];
  Curry._2(DOM$ReView.useLayoutEffect, (function (param) {
          setTimeout((function (param) {
                  return Curry._1(setState, (function (param) {
                                return finalImage;
                              }));
                }), 5000);
          
        }), state);
  var match$1 = Curry._1(DOM$ReView.useState, (function (param) {
          return testContext;
        }));
  var setContext = match$1[1];
  console.log(state);
  return Curry._2(DOM$ReView.DomCore.ErrorBoundary.make, {
              key: undefined,
              ref: undefined
            }, {
              onError: (function (value, trace) {
                  console.log(value);
                  console.log(trace);
                  return Curry._1(setContext, (function (param) {
                                return imageContext;
                              }));
                }),
              children: [Curry._2(DOM$ReView.DomCore.Context.Provider.make, {
                      key: undefined,
                      ref: undefined
                    }, {
                      context: match$1[0],
                      value: state,
                      children: [Curry._2(Content.make, {
                              key: undefined,
                              ref: undefined
                            }, undefined)]
                    })]
            });
}

var App = Curry._1(DOM$ReView.DomCore.Component, {
      make: make$4,
      name: "App"
    });

Curry._2(DOM$ReView.render, Curry._2(App.make, {
          key: undefined,
          ref: undefined
        }, undefined), document.body);

exports.initialImage = initialImage;
exports.finalImage = finalImage;
exports.imageContext = imageContext;
exports.testContext = testContext;
exports.Test = Test;
exports.$$Image = $$Image;
exports.ImageSrc = ImageSrc;
exports.ImageGroup = ImageGroup;
exports.Content = Content;
exports.App = App;
/* imageContext Not a pure module */
