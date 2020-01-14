// Generated by BUCKLESCRIPT, PLEASE EDIT WITH CARE
'use strict';

var Curry = require("bs-platform/lib/js/curry.js");
var Node$ReView = require("./Node.bs.js");
var State$ReView = require("./Hooks/State.bs.js");
var Render$ReView = require("./Render.bs.js");
var Fragment$ReView = require("./Components/Fragment.bs.js");
var Component$ReView = require("./Component.bs.js");

var make = Component$ReView.make("Test", (function (param) {
        var match = State$ReView.use(100);
        console.log(match[0]);
        Curry._1(match[1], (function (param) {
                return 1000;
              }));
        return ;
      }));

var Test = {
  make: make
};

var make$1 = Component$ReView.make("App", (function (param) {
        console.log("rendered again :/");
        return Fragment$ReView.make(param.children);
      }));

var App = {
  make: make$1
};

var root = Node$ReView.make(undefined, undefined);

Render$ReView.call(Curry._1(make$1, {
          children: /* :: */[
            Curry._1(make, /* () */0),
            /* :: */[
              Curry._1(make, /* () */0),
              /* [] */0
            ]
          ]
        }), root);

var dir = (
  function dir(object) {
    console.dir(object, { depth: null });
  }
);

setTimeout((function (param) {
        Curry._1(dir, root);
        Render$ReView.call(Curry._1(make$1, {
                  children: /* :: */[
                    Curry._1(make, /* () */0),
                    /* :: */[
                      Curry._1(make, /* () */0),
                      /* [] */0
                    ]
                  ]
                }), root);
        setTimeout((function (param) {
                return Curry._1(dir, root);
              }));
        return /* () */0;
      }));

exports.Test = Test;
exports.App = App;
exports.root = root;
exports.dir = dir;
/* make Not a pure module */