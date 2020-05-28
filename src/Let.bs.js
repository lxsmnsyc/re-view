// Generated by BUCKLESCRIPT, PLEASE EDIT WITH CARE
'use strict';

var Curry = require("bs-platform/lib/js/curry.js");
var Caml_option = require("bs-platform/lib/js/caml_option.js");

function let_(value, mapper) {
  if (value !== undefined) {
    return Curry._1(mapper, Caml_option.valFromOption(value));
  }
  
}

var $$Option = {
  let_: let_
};

function let_$1(value, mapper) {
  if (value !== undefined) {
    Curry._1(mapper, Caml_option.valFromOption(value));
    return ;
  }
  
}

var OptionUnit = {
  let_: let_$1
};

function $pipe$pipe$great(value, error) {
  if (value !== undefined) {
    return Caml_option.valFromOption(value);
  }
  throw error;
}

function $pipe$pipe$less(value, fallback) {
  if (value !== undefined) {
    return Caml_option.valFromOption(value);
  } else {
    return fallback;
  }
}

exports.$$Option = $$Option;
exports.OptionUnit = OptionUnit;
exports.$pipe$pipe$great = $pipe$pipe$great;
exports.$pipe$pipe$less = $pipe$pipe$less;
/* No side effect */
