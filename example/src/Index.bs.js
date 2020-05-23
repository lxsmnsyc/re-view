'use strict';

var Curry = require("bs-platform/lib/js/curry.js");
var DOM$ReView = require("re-view/src/DOM.bs.js");

var initialImage = "https://images.dog.ceo/breeds/frise-bichon/jh-ezio-3.jpg";

var finalImage = "https://images.dog.ceo/breeds/spaniel-cocker/n02102318_2984.jpg";

function make(param, param$1) {
  var match = Curry._1(DOM$ReView.useState, (function (param) {
          return initialImage;
        }));
  var setState = match[1];
  var state = match[0];
  Curry._2(DOM$ReView.useEffect, (function (param) {
          Curry._1(setState, (function (param) {
                  return finalImage;
                }));
          
        }), state);
  return DOM$ReView.$$Element.make("img", {
              key: undefined,
              ref: /* None */0
            }, {
              attributes: {
                src: state
              },
              children: undefined
            });
}

var $$Image = Curry._1(DOM$ReView.DomCore.Component, {
      make: make,
      name: "Image"
    });

function make$1(param, param$1) {
  return DOM$ReView.$$Element.make("div", {
              key: undefined,
              ref: /* None */0
            }, {
              attributes: { },
              children: [
                Curry._2($$Image.make, {
                      key: undefined,
                      ref: /* None */0
                    }, undefined),
                Curry._2($$Image.make, {
                      key: undefined,
                      ref: /* None */0
                    }, undefined)
              ]
            });
}

var ImageGroup = Curry._1(DOM$ReView.DomCore.Component, {
      make: make$1,
      name: "ImageGroup"
    });

function make$2(param, param$1) {
  return Curry._2(DOM$ReView.DomCore.Fragment.make, {
              key: undefined,
              ref: /* None */0
            }, {
              children: [
                Curry._2(ImageGroup.make, {
                      key: undefined,
                      ref: /* None */0
                    }, undefined),
                Curry._2(ImageGroup.make, {
                      key: undefined,
                      ref: /* None */0
                    }, undefined)
              ]
            });
}

var App = Curry._1(DOM$ReView.DomCore.Component, {
      make: make$2,
      name: "App"
    });

Curry._2(DOM$ReView.render, Curry._2(App.make, {
          key: undefined,
          ref: /* None */0
        }, undefined), document.body);

exports.initialImage = initialImage;
exports.finalImage = finalImage;
exports.$$Image = $$Image;
exports.ImageGroup = ImageGroup;
exports.App = App;
/* Image Not a pure module */
