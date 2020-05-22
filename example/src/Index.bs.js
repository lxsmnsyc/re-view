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
  console.log(state);
  Curry._2(DOM$ReView.useEffect, (function (param) {
          Curry._1(setState, (function (param) {
                  return finalImage;
                }));
          
        }), state);
  return DOM$ReView.$$Element.make("div", {
              key: undefined,
              ref: /* None */0
            }, {
              attributes: { },
              children: [
                DOM$ReView.$$Element.make("img", {
                      key: undefined,
                      ref: /* None */0
                    }, {
                      attributes: {
                        src: state
                      },
                      children: undefined
                    }),
                DOM$ReView.$$Element.make("img", {
                      key: undefined,
                      ref: /* None */0
                    }, {
                      attributes: {
                        src: state
                      },
                      children: undefined
                    })
              ]
            });
}

var App = Curry._1(DOM$ReView.DomCore.Component, {
      make: make
    });

Curry._2(DOM$ReView.render, Curry._2(App.make, {
          key: undefined,
          ref: /* None */0
        }, undefined), document.body);

var state = initialImage;

exports.initialImage = initialImage;
exports.finalImage = finalImage;
exports.state = state;
exports.App = App;
/* App Not a pure module */
